/**
 * \file dnn/src/x86/convolution/avx/convolution_conv_fh7_avx.cpp
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
#define SIMD_H1                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
        }                                                                              \
    } while (0)

#define SIMD_H2                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
        }                                                                              \
    } while (0)

#define SIMD_H3                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            __m256 res2;                                                               \
            res2 = _mm256_loadu_ps(dst_dd + 2 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 8 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
            _mm256_storeu_ps(dst_dd + 2 * dst_w, res2);                                \
        }                                                                              \
    } while (0)

#define SIMD_H4                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            __m256 res2;                                                               \
            res2 = _mm256_loadu_ps(dst_dd + 2 * dst_w);                                \
            __m256 res3;                                                               \
            res3 = _mm256_loadu_ps(dst_dd + 3 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 8 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 9 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
            _mm256_storeu_ps(dst_dd + 2 * dst_w, res2);                                \
            _mm256_storeu_ps(dst_dd + 3 * dst_w, res3);                                \
        }                                                                              \
    } while (0)

#define SIMD_H5                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            __m256 res2;                                                               \
            res2 = _mm256_loadu_ps(dst_dd + 2 * dst_w);                                \
            __m256 res3;                                                               \
            res3 = _mm256_loadu_ps(dst_dd + 3 * dst_w);                                \
            __m256 res4;                                                               \
            res4 = _mm256_loadu_ps(dst_dd + 4 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 8 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 9 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 10 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
            _mm256_storeu_ps(dst_dd + 2 * dst_w, res2);                                \
            _mm256_storeu_ps(dst_dd + 3 * dst_w, res3);                                \
            _mm256_storeu_ps(dst_dd + 4 * dst_w, res4);                                \
        }                                                                              \
    } while (0)

#define SIMD_H6                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            __m256 res2;                                                               \
            res2 = _mm256_loadu_ps(dst_dd + 2 * dst_w);                                \
            __m256 res3;                                                               \
            res3 = _mm256_loadu_ps(dst_dd + 3 * dst_w);                                \
            __m256 res4;                                                               \
            res4 = _mm256_loadu_ps(dst_dd + 4 * dst_w);                                \
            __m256 res5;                                                               \
            res5 = _mm256_loadu_ps(dst_dd + 5 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 8 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 9 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 10 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 11 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
            _mm256_storeu_ps(dst_dd + 2 * dst_w, res2);                                \
            _mm256_storeu_ps(dst_dd + 3 * dst_w, res3);                                \
            _mm256_storeu_ps(dst_dd + 4 * dst_w, res4);                                \
            _mm256_storeu_ps(dst_dd + 5 * dst_w, res5);                                \
        }                                                                              \
    } while (0)

#define SIMD_H7                                                                        \
    do {                                                                               \
        const size_t sh = dh;                                                          \
        const float* src_d = src + sh * src_w;                                         \
        float* dst_d = dst + dh * dst_w;                                               \
        size_t dw = dst_w_beg;                                                         \
        for (; dw < dst_w_end; dw += 8) {                                              \
            const size_t sw = dw;                                                      \
            float* dst_dd = dst_d + dw;                                                \
            __m256 tmp0;                                                               \
            __m256 tmp1;                                                               \
            __m256 res0;                                                               \
            res0 = _mm256_loadu_ps(dst_dd + 0 * dst_w);                                \
            __m256 res1;                                                               \
            res1 = _mm256_loadu_ps(dst_dd + 1 * dst_w);                                \
            __m256 res2;                                                               \
            res2 = _mm256_loadu_ps(dst_dd + 2 * dst_w);                                \
            __m256 res3;                                                               \
            res3 = _mm256_loadu_ps(dst_dd + 3 * dst_w);                                \
            __m256 res4;                                                               \
            res4 = _mm256_loadu_ps(dst_dd + 4 * dst_w);                                \
            __m256 res5;                                                               \
            res5 = _mm256_loadu_ps(dst_dd + 5 * dst_w);                                \
            __m256 res6;                                                               \
            res6 = _mm256_loadu_ps(dst_dd + 6 * dst_w);                                \
            for (size_t fw = 0; fw < flt_w; ++fw) {                                    \
                const float* src_dd = src_d + sw + fw;                                 \
                __m256 vf0 = _mm256_broadcast_ss(&filter[0 * flt_w + flt_w - fw - 1]); \
                __m256 vf1 = _mm256_broadcast_ss(&filter[1 * flt_w + flt_w - fw - 1]); \
                __m256 vf2 = _mm256_broadcast_ss(&filter[2 * flt_w + flt_w - fw - 1]); \
                __m256 vf3 = _mm256_broadcast_ss(&filter[3 * flt_w + flt_w - fw - 1]); \
                __m256 vf4 = _mm256_broadcast_ss(&filter[4 * flt_w + flt_w - fw - 1]); \
                __m256 vf5 = _mm256_broadcast_ss(&filter[5 * flt_w + flt_w - fw - 1]); \
                __m256 vf6 = _mm256_broadcast_ss(&filter[6 * flt_w + flt_w - fw - 1]); \
                tmp0 = _mm256_loadu_ps(src_dd + 0 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 1 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 2 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 3 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 4 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 5 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 6 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res0 = _mm256_add_ps(res0, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf6);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 7 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res1 = _mm256_add_ps(res1, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf5);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 8 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res2 = _mm256_add_ps(res2, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf4);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 9 * src_w);                            \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res3 = _mm256_add_ps(res3, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf3);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 10 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res4 = _mm256_add_ps(res4, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf2);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 11 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res5 = _mm256_add_ps(res5, tmp1);                                      \
                tmp1 = _mm256_mul_ps(tmp0, vf1);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
                tmp0 = _mm256_loadu_ps(src_dd + 12 * src_w);                           \
                tmp1 = _mm256_mul_ps(tmp0, vf0);                                       \
                res6 = _mm256_add_ps(res6, tmp1);                                      \
            }                                                                          \
            _mm256_storeu_ps(dst_dd + 0 * dst_w, res0);                                \
            _mm256_storeu_ps(dst_dd + 1 * dst_w, res1);                                \
            _mm256_storeu_ps(dst_dd + 2 * dst_w, res2);                                \
            _mm256_storeu_ps(dst_dd + 3 * dst_w, res3);                                \
            _mm256_storeu_ps(dst_dd + 4 * dst_w, res4);                                \
            _mm256_storeu_ps(dst_dd + 5 * dst_w, res5);                                \
            _mm256_storeu_ps(dst_dd + 6 * dst_w, res6);                                \
        }                                                                              \
    } while (0)

#include <algorithm>
#include "src/x86/avx_helper.h"

#include "../convolution_direct_special_cases.h"

namespace megdnn {
namespace x86 {
namespace detail {

void convolution_conv_fh7_avx(
        const float* src, const float* filter, float* dst, const size_t src_h,
        const size_t src_w, const size_t dst_h, const size_t dst_w,
        const size_t flt_w) {
    (void)src_h;
    const size_t dst_h_beg = 0;
    const size_t dst_h_end = dst_h;
    const size_t dst_w_beg = 0;
    const size_t dst_w_end = dst_w;

    size_t dh = dst_h_beg;
    for (; dh + 7 <= dst_h_end; dh += 7) {
        SIMD_H7;
    }
    switch (dst_h_end - dh) {
        case 1:
            SIMD_H1;
            break;
        case 2:
            SIMD_H2;
            break;
        case 3:
            SIMD_H3;
            break;
        case 4:
            SIMD_H4;
            break;
        case 5:
            SIMD_H5;
            break;
        case 6:
            SIMD_H6;
            break;
    }
}

}  // namespace detail
}  // namespace x86
}  // namespace megdnn
#undef SIMD_H1
#undef SIMD_H2
#undef SIMD_H3
#undef SIMD_H4
#undef SIMD_H5
#undef SIMD_H6
#undef SIMD_H7
