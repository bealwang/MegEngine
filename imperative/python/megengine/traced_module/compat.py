# MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
#
# Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

import numpy as np

from megengine.functional.tensor import zeros

from ..core.ops.builtin import BatchNorm
from .expr import CallMethod, Constant
from .node import TensorNode
from .serialization import (
    register_functional_loader,
    register_module_loader,
    register_opdef_loader,
    register_tensor_method_loader,
)


"""
# Expr loaders examples

from ..core.ops.builtin import Elemwise

@register_opdef_loader(Elemwise)
def add_opdef_loader(expr):
    if expr.opdef_state["mode"] == "ADD":
        expr.opdef_state["mode"] == "MUL"
        node = expr.inputs[1]
        astype_expr = CallMethod(node, "astype")
        oup = TensorNode(
            astype_expr,
            shape=node.shape,
            dtype=expr.inputs[0].dtype,
            qparams=node.qparams,
        )

        astype_expr.set_args_kwargs(node, expr.inputs[0].dtype)
        astype_expr.return_val = (oup,)
        expr.inputs[1] = oup


@register_functional_loader(("megengine.functional.nn", "conv2d"))
def conv2df_loader(expr):
    # expr.func = ("megengine.functional.nn","conv2d")
    kwargs = expr.kwargs
    orig_weight = expr.named_args["weight"]

    astype_expr = CallMethod(orig_weight, "astype")
    oup = TensorNode(
        astype_expr,
        shape=orig_weight.shape,
        dtype=orig_weight.dtype,
        qparams=orig_weight.qparams,
    )

    astype_expr.set_args_kwargs(orig_weight, expr.named_args["inp"].dtype)
    astype_expr.return_val = (oup,)

    expr.set_arg("weight", oup)


@register_module_loader(("megengine.module.conv", "Conv2d"))
def conv2dm_loader(expr):
    module = expr.inputs[0].owner
    args = list(expr.args)
    orig_inp = args[1]
    astype_expr = CallMethod(orig_inp, "astype")
    oup = TensorNode(
        astype_expr,
        shape=orig_inp.shape,
        dtype=orig_inp.dtype,
        qparams=orig_inp.qparams,
    )
    astype_expr.set_args_kwargs(orig_inp, module.weight.dtype)
    astype_expr.return_val = (oup,)
    args[1] = oup
    expr.set_args_kwargs(*args)


@register_tensor_method_loader("__add__")
def add_loader(expr):
    args = list(expr.args)
    if not isinstance(args[1], TensorNode):
        args[1] = tensor(args[1])
        node = Constant(args[1], "const").outputs[0]

        astype_expr = CallMethod(node, "astype")
        oup = TensorNode(
            astype_expr, shape=node.shape, dtype=node.dtype, qparams=node.qparams,
        )

        astype_expr.set_args_kwargs(node, expr.inputs[0].dtype)
        astype_expr.return_val = (oup,)
        args[1] = oup
        expr.set_args_kwargs(*args)
"""


@register_module_loader(
    ("megengine.module.batchnorm", "BatchNorm1d"),
    ("megengine.module.batchnorm", "BatchNorm2d"),
    ("megengine.module.batchnorm", "SyncBatchNorm"),
)
def bn2d_module_loader(expr):
    # mge 1.6
    if not hasattr(expr, "version"):
        module = expr.inputs[0].owner
        if not hasattr(module, "param_dim"):
            module.param_dim = "dim_1c11"


@register_module_loader(
    ("megengine.module.conv_bn", "ConvBn2d"),
    ("megengine.module.conv_bn", "ConvBnRelu2d"),
    ("megengine.module.qat.conv_bn", "ConvBn2d"),
    ("megengine.module.qat.conv_bn", "ConvBnRelu2d"),
)
def convbn2d_module_loader(expr):
    # mge 1.6
    if not hasattr(expr, "version"):
        module = expr.inputs[0].owner
        if not hasattr(module.bn, "param_dim"):
            module.bn.param_dim = "dim_1c11"


@register_opdef_loader(BatchNorm)
def bn_opdef_loader(expr):
    # mge 1.6
    if not hasattr(expr, "version") and len(expr.outputs) != 6:
        assert len(expr.outputs) == 5
        output = expr.outputs[-1]
        oup = TensorNode(expr, shape=(0,), dtype=None, qparams=output._qparams,)
        expr.outputs.insert(4, oup)


@register_functional_loader(
    ("megengine.functional.tensor", "ones"), ("megengine.functional.tensor", "zeros")
)
def tensor_gen_func_loader(expr):
    if hasattr(expr, "version") and expr.version == "1.7.0":
        expr.set_args_kwargs(expr.args[0], dtype=expr.args[1], device=expr.args[2])
    if not hasattr(expr, "version"):
        # compatiable for version 1.6
        shape = expr.args[0] if len(expr.args) > 0 else expr.kwargs["shape"]

        if len(expr.args) > 1:
            dtype = expr.args[1]
        elif "dtype" in expr.kwargs:
            dtype = expr.kwargs["dtype"]
        else:
            dtype = "float32"

        if len(expr.args) > 2:
            device = expr.args[2]
        elif "device" in expr.kwargs:
            device = expr.kwargs["device"]
        else:
            device = None
        expr.set_args_kwargs(shape, dtype=dtype, device=device)
