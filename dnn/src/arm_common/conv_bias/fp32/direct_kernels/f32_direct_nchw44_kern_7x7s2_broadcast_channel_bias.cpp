/**
 * \file
 * dnn/src/arm_common/conv_bias/fp32/direct_kernels/f32_direct_nchw44_kern_7x7s2_broadcast_channel_bias.cpp
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 */
#include "src/arm_common/conv_bias/fp32/direct_kernels/f32_direct_nchw44_kern_common_s2.h"
INSTANTIATION_CONV_S2_BROADCAST_CHANNEL_BIAS(7);
// vim: syntax=cpp.doxygen
