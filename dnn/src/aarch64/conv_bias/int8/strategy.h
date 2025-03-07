/**
 * \file dnn/src/aarch64/conv_bias/int8/strategy.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
#pragma once
#include "src/fallback/matrix_mul/gemm_common.h"

namespace megdnn {
namespace aarch64 {
namespace matmul {

/**
 * \brief base strategy of gemm.
 *
 * \name gemm_<type>_<block>_biasmode_nolinemode
 */
MEGDNN_REG_GEMM_STRATEGY_WITH_WRITEBACK(
        dt_int8, dt_int8, dt_int32, 4, 4, 16, false, true, gemm_s8_4x4_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_4x4_nobias_relu, gemm_s8_4x4_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_4x4_nobias_hswish, gemm_s8_4x4_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_4x4_bias_channel_identity, gemm_s8_4x4_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_4x4_bias_channel_relu, gemm_s8_4x4_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_4x4_bias_channel_hswish, gemm_s8_4x4_nobias_identity);
#if MGB_ENABLE_DOT
MEGDNN_REG_GEMM_STRATEGY_WITH_WRITEBACK(
        dt_int8, dt_int8, dt_int32, 8, 12, 4, false, true,
        gemm_s8_8x12_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_8x12_nobias_relu, gemm_s8_8x12_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_8x12_nobias_hswish, gemm_s8_8x12_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_8x12_bias_channel_identity, gemm_s8_8x12_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_8x12_bias_channel_relu, gemm_s8_8x12_nobias_identity);

MEGDNN_REG_GEMM_STRATEGY_WITH_SUPER(
        gemm_s8_8x12_bias_channel_hswish, gemm_s8_8x12_nobias_identity);
#endif

}  // namespace matmul
}  // namespace aarch64
}  // namespace megdnn

// vim: syntax=cpp.doxygen
