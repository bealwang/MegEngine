# MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
#
# Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

from ... import functional as F
from ... import module as M
from ...core.ops.builtin import GetVarShape
from ...logger import get_logger
from ...tensor import Tensor
from ..expr import Constant, Expr, is_apply_def, is_constant, is_getattr
from ..node import Node, NodeMixin, TensorNode
from .matcher import PatternMatcher
from .pass_base import BackwardPass, ForwardPass, register_pass
from .pattern import is_op
from .utils import get_const_value

logger = get_logger(__name__)


def _as_const_node(x):
    node = Constant.make(x)
    NodeMixin.wrap(x, node)
    return node


@register_pass("AttrToConstant")
class AttrToConstant(BackwardPass):
    r"""Convert :class:`~.GetAttr` to :class:`~.Constant` expr."""
    name = "AttrToConstant"
    run_once = True

    def run_transform(self, expr: Expr):
        if not (is_getattr(expr) and isinstance(expr.outputs[0], TensorNode)):
            return expr
        graph = expr.top_graph
        value = get_const_value(expr)
        orig_node = expr.outputs[0]
        name = orig_node.name
        with graph.insert_exprs(expr):
            const_node = _as_const_node(value)
        graph.replace_node({orig_node: const_node})
        graph.compile()
        const_node.name = name
        return const_node.expr


@register_pass("FixInputShape")
class FixInputShape(BackwardPass):
    name = "FixInputShape"
    run_once = True

    def run_transform(self, expr: Expr):
        if not is_apply_def(expr, GetVarShape):
            return expr
        shape = Tensor(expr.inputs[0].shape, dtype="int32")
        graph = expr.top_graph
        with graph.insert_exprs(expr):
            const_shape = _as_const_node(shape)
        graph.replace_node({expr.outputs[0]: const_shape})
        graph.compile()
        const_shape.name = expr.outputs[0].name
        return const_shape.expr


@register_pass("FlodConstant")
class FlodConstant(ForwardPass):
    r"""Constant folding."""
    name = "FlodConstant"
    required_pass = ["AttrToConstant"]
    run_once = False

    def run_transform(self, expr: Expr):
        if len(expr.inputs) == 0 or any(not is_constant(n.expr) for n in expr.inputs):
            return expr
        const_var = expr.interpret(*[get_const_value(n.expr) for n in expr.inputs])[0]
        graph = expr.top_graph
        with graph.insert_exprs(expr):
            const_node = _as_const_node(const_var)
        graph.replace_node({expr.outputs[0]: const_node})
        graph.compile()
        const_node.name = expr.outputs[0].name
        return const_node.expr


@register_pass("NormElemWise")
class NormElemWise(BackwardPass):
    r"""Transform add/sub or mul/div expr to add-only or mul-only chains.
    
    For example, the following code

    .. code-block::

        b = 1 - a
        c = 2 * b
        d = 1 / c

    will be changed to

    .. code-block::
    
        a1 = F.neg(a)
        b = a1 + 1
        c = b * 2
        d = F.pow(d, -1)
    """
    name = "NormElemWise"
    required_pass = ["FlodConstant"]
    run_once = False

    def __init__(self,):
        super().__init__()
        self.pattern = is_op(F.add)
        for op in [F.sub, F.mul, F.div]:
            self.pattern |= is_op(op)
        for op in ["__add__", "__iadd__", "__radd__"]:
            self.pattern |= is_op(op)
        for op in ["__sub__", "__isub__", "__rsub__"]:
            self.pattern |= is_op(op)
        for op in ["__mul__", "__imul__", "__rmul__"]:
            self.pattern |= is_op(op)
        for op in ["__truediv__", "__itruediv__", "__rtruediv__"]:
            self.pattern |= is_op(op)

    def run_transform(self, expr: Expr):

        matcher = PatternMatcher()
        if not matcher.match(self.pattern, expr):
            return expr

        pattern = matcher.matched_patterns[0]
        target = pattern.target
        cofee, left_node, right_node = 1, None, None
        if len(expr.inputs) == 1 and target not in ["__add__", "__mul__"]:
            left_node = expr.inputs[0]
            right_node = expr.const_val[0][-1]
            if target in ["__rsub__", "__rtruediv__"]:
                cofee = -1
            if target in [F.sub, F.div] and left_node is not expr.kwargs["x"]:
                cofee = -1
        elif len(expr.inputs) == 2 and (
            target not in ["__add__", "__mul__"] or is_constant(expr.inputs[0].expr)
        ):
            left_node, right_node = expr.inputs
            if target in ["__rsub__", "__rtruediv__"]:
                left_node, right_node = right_node, left_node
            if target in [F.sub, F.div] and left_node is not expr.kwargs["x"]:
                left_node, right_node = right_node, left_node
            if is_constant(left_node.expr):
                left_node, right_node = right_node, left_node
                cofee = -1

        if left_node is None:
            return expr

        if isinstance(right_node, TensorNode):
            right_node = get_const_value(right_node.expr, right_node)

        graph = expr.top_graph
        with graph.insert_exprs():
            if target in ["__mul__", "__imul__", "__rmul__", F.mul]:
                out_node = left_node * right_node
            elif target in ["__add__", "__iadd__", "__radd__", F.add]:
                out_node = left_node + right_node
            elif target in ["__sub__", "__isub__", "__rsub__", F.sub]:
                if cofee == -1:
                    left_node = F.neg(left_node)
                else:
                    if isinstance(right_node, TensorNode):
                        right_node = F.neg(right_node)
                    else:
                        right_node = -1 * right_node
                out_node = left_node + right_node
            elif target in ["__truediv__", "__itruediv__", "__rtruediv__", F.div]:
                if cofee == -1:
                    left_node = F.pow(left_node, -1)
                else:
                    if isinstance(right_node, TensorNode):
                        right_node = F.pow(right_node, -1)
                    else:
                        right_node = 1 / right_node
                out_node = left_node * right_node

        graph.replace_node({expr.outputs[0]: out_node})
        graph.compile()
        return out_node.expr
