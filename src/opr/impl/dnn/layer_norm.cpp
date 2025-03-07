/**
 * \file src/opr/impl/dnn/layer_norm.cpp
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 */

#include "megbrain/opr/dnn/layer_norm.h"

#include "megbrain/graph/grad_impl.h"
#include "megbrain/opr/internal/out_shape_by_sym_var.h"
#include "megbrain/opr/utility.h"

#include "../internal/megdnn_opr_wrapper.inl"

using namespace mgb;
using namespace opr;

/* ==================== LayerNormForward  ==================== */
MGB_DYN_TYPE_OBJ_FINAL_IMPL(LayerNormForward);

LayerNormForward::LayerNormForward(
        VarNode* data, VarNode* weight, VarNode* bias, const Param& param,
        const OperatorNodeConfig& config)
        : Super{data->owner_graph(), config, "layer_norm", {data, weight, bias}} {
    init_megdnn_opr(*this, param);

    add_input({data, weight, bias});
    output(0)->dtype(data->dtype());
    output(1)->dtype(dtype::Float32());
    output(2)->dtype(dtype::Float32());
}

LayerNormForward::LayerNormForward(
        VarNode* data, const Param& param, const OperatorNodeConfig& config)
        : Super{data->owner_graph(), config, "layer_norm", {data}} {
    init_megdnn_opr(*this, param);

    add_input({data});
    output(0)->dtype(data->dtype());
    output(1)->dtype(dtype::Float32());
    output(2)->dtype(dtype::Float32());
}

SymbolVarArray LayerNormForward::make(
        SymbolVar data, SymbolVar weight, SymbolVar bias, const Param& param,
        const OperatorNodeConfig& config) {
    auto outs = data.node()
                        ->owner_graph()
                        ->insert_opr(std::make_unique<LayerNormForward>(
                                data.node(), weight.node(), bias.node(), param, config))
                        ->output();
    SymbolVarArray ret;
    for (auto&& out : outs) {
        ret.emplace_back(out);
    }
    return ret;
}

SymbolVarArray LayerNormForward::make(
        SymbolVar data, const Param& param, const OperatorNodeConfig& config) {
    auto outs = data.node()
                        ->owner_graph()
                        ->insert_opr(std::make_unique<LayerNormForward>(
                                data.node(), param, config))
                        ->output();
    SymbolVarArray ret;
    for (auto&& out : outs) {
        ret.emplace_back(out);
    }
    return ret;
}

void LayerNormForward::get_output_var_shape(
        const TensorShapeArray& inp_shape, TensorShapeArray& out_shape) const {
    uint64_t normalized_dim = param().normalized_dim;
    out_shape[0] = inp_shape[0];
    TensorShape unnormalized_shape;
    unnormalized_shape.ndim = inp_shape[0].ndim - normalized_dim;
    for (size_t i = 0; i < unnormalized_shape.ndim; ++i) {
        unnormalized_shape.shape[i] = inp_shape[0].shape[i];
    }
    out_shape[1] = unnormalized_shape;
    out_shape[2] = unnormalized_shape;
}

size_t LayerNormForward::get_workspace_size_bytes(
        const TensorShapeArray& input_shapes,
        const TensorShapeArray& output_shapes) const {
    return 0;
}

void LayerNormForward::scn_do_execute() {
    if (param().affine) {
        megdnn_opr()->exec(
                input(0)->dev_tensor().as_megdnn(), input(1)->dev_tensor().as_megdnn(),
                input(2)->dev_tensor().as_megdnn(), output(0)->dev_tensor().as_megdnn(),
                output(1)->dev_tensor().as_megdnn(),
                output(2)->dev_tensor().as_megdnn(), {});
    } else {
        megdnn_opr()->exec(
                input(0)->dev_tensor().as_megdnn(), {}, {},
                output(0)->dev_tensor().as_megdnn(),
                output(1)->dev_tensor().as_megdnn(),
                output(2)->dev_tensor().as_megdnn(), {});
    }
}

#if MGB_ENABLE_GRAD
MGB_IMPL_OPR_GRAD(LayerNormForward) {
    auto p = opr.param();
    SymbolVarArray grad;
    VarNodeArray ret;
    if (p.affine) {
        mgb_assert(wrt_idx < 3, "wrt_idx %zu is out of range", wrt_idx);
        grad = LayerNormBackward::make(
                out_grad[0], opr.input(0), opr.input(1), opr.output(1), opr.output(2),
                opr.param());
    } else {
        mgb_assert(wrt_idx < 1, "wrt_idx %zu is out of range", wrt_idx);
        grad = LayerNormBackward::make(
                out_grad[0], opr.input(0), opr.output(1), opr.output(2), opr.param());
    }

    uint32_t nr_ret = p.affine ? 3 : 1;
    for (uint32_t i = 0; i < nr_ret; ++i) {
        ret.push_back(grad[i].node());
    }
    return ret;
}
#endif

/* ==================== LayerNormBackward ==================== */
MGB_DYN_TYPE_OBJ_FINAL_IMPL(LayerNormBackward);

LayerNormBackward::LayerNormBackward(
        VarNode* diff, VarNode* data, VarNode* weight, VarNode* mean, VarNode* rstd,
        const Param& param, const OperatorNodeConfig& config)
        : Super({diff->owner_graph(),
                 config,
                 "layer_norm_backward",
                 {diff, data, weight, mean, rstd}},
                0, true) {
    init_megdnn_opr(*this, param);
    add_input({diff, data, weight, mean, rstd});
}

LayerNormBackward::LayerNormBackward(
        VarNode* diff, VarNode* data, VarNode* mean, VarNode* rstd, const Param& param,
        const OperatorNodeConfig& config)
        : Super({diff->owner_graph(),
                 config,
                 "layer_norm_backward",
                 {diff, data, mean, rstd}},
                0, true) {
    init_megdnn_opr(*this, param);
    add_input({diff, data, mean, rstd});
    auto mark_empty_var = [&](VarNode* var) {
        var->add_flag(VarNode::Flag::ALLOW_EMPTY_SHAPE)
                .add_flag(VarNode::Flag::VOLATILE_CONTENT);
    };
    mark_empty_var(output(1));
    mark_empty_var(output(2));
}

SymbolVarArray LayerNormBackward::make(
        SymbolVar diff, SymbolVar data, SymbolVar weight, SymbolVar mean,
        SymbolVar rstd, const Param& param, const OperatorNodeConfig& config) {
    auto outs = diff.node()
                        ->owner_graph()
                        ->insert_opr(std::make_unique<LayerNormBackward>(
                                diff.node(), data.node(), weight.node(), mean.node(),
                                rstd.node(), param, config))
                        ->output();
    SymbolVarArray ret;
    for (auto&& out : outs) {
        ret.emplace_back(out);
    }
    return ret;
}

SymbolVarArray LayerNormBackward::make(
        SymbolVar diff, SymbolVar data, SymbolVar mean, SymbolVar rstd,
        const Param& param, const OperatorNodeConfig& config) {
    auto outs = diff.node()
                        ->owner_graph()
                        ->insert_opr(std::make_unique<LayerNormBackward>(
                                diff.node(), data.node(), mean.node(), rstd.node(),
                                param, config))
                        ->output();
    SymbolVarArray ret;
    for (auto&& out : outs) {
        ret.emplace_back(out);
    }
    return ret;
}

void LayerNormBackward::init_output_static_infer_desc() {
    using namespace cg::static_infer;
    auto&& mgr = owner_graph()->static_infer_manager();
    mgr.register_shape_infer(output(0), ShapeInferDesc::make_identity(input(1)));
    if (param().affine) {
        mgr.register_shape_infer(output(1), ShapeInferDesc::make_identity(input(2)));
        mgr.register_shape_infer(output(2), ShapeInferDesc::make_identity(input(2)));
    } else {
        TensorShape empty;
        empty.ndim = 0;
        mgr.register_shape_infer(output(1), ShapeInferDesc::make_const(empty));
        mgr.register_shape_infer(output(2), ShapeInferDesc::make_const(empty));
    }
    this->init_output_static_infer_desc_workspace(false);
}

void LayerNormBackward::init_output_dtype() {
    output(0)->dtype(input(1)->dtype());
    output(1)->dtype(input(2)->dtype());
    output(2)->dtype(input(2)->dtype());
}

size_t LayerNormBackward::get_workspace_size_bytes(
        const TensorShapeArray& input_shapes,
        const TensorShapeArray& output_shapes) const {
    return 0;
}

void LayerNormBackward::scn_do_execute() {
    if (param().affine) {
        megdnn_opr()->exec(
                input(0)->dev_tensor().as_megdnn(), input(1)->dev_tensor().as_megdnn(),
                input(2)->dev_tensor().as_megdnn(), input(3)->dev_tensor().as_megdnn(),
                input(4)->dev_tensor().as_megdnn(), output(0)->dev_tensor().as_megdnn(),
                output(1)->dev_tensor().as_megdnn(),
                output(2)->dev_tensor().as_megdnn(), {});
    } else {
        megdnn_opr()->exec(
                input(0)->dev_tensor().as_megdnn(), input(1)->dev_tensor().as_megdnn(),
                {}, input(2)->dev_tensor().as_megdnn(),
                input(3)->dev_tensor().as_megdnn(), output(0)->dev_tensor().as_megdnn(),
                {}, {}, {});
    }
}

// vim: syntax=cpp.doxygen foldmethod=marker foldmarker=f{{{,f}}}
