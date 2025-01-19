// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_GL_FORMAT_HPP
#define ANTKEEPER_GL_FORMAT_HPP

#include <cstdint>

namespace gl {

/// Image and vertex formats.
enum class format: std::uint32_t
{
	undefined,
    r4g4_unorm_pack8,
    r4g4b4a4_unorm_pack16,
    b4g4r4a4_unorm_pack16,
    r5g6b5_unorm_pack16,
    b5g6r5_unorm_pack16,
    r5g5b5a1_unorm_pack16,
    b5g5r5a1_unorm_pack16,
    a1r5g5b5_unorm_pack16,
    r8_unorm,
    r8_snorm,
    r8_uscaled,
    r8_sscaled,
    r8_uint,
    r8_sint,
    r8_srgb,
    r8g8_unorm,
    r8g8_snorm,
    r8g8_uscaled,
    r8g8_sscaled,
    r8g8_uint,
    r8g8_sint,
    r8g8_srgb,
    r8g8b8_unorm,
    r8g8b8_snorm,
    r8g8b8_uscaled,
    r8g8b8_sscaled,
    r8g8b8_uint,
    r8g8b8_sint,
    r8g8b8_srgb,
    b8g8r8_unorm,
    b8g8r8_snorm,
    b8g8r8_uscaled,
    b8g8r8_sscaled,
    b8g8r8_uint,
    b8g8r8_sint,
    b8g8r8_srgb,
    r8g8b8a8_unorm,
    r8g8b8a8_snorm,
    r8g8b8a8_uscaled,
    r8g8b8a8_sscaled,
    r8g8b8a8_uint,
    r8g8b8a8_sint,
    r8g8b8a8_srgb,
    b8g8r8a8_unorm,
    b8g8r8a8_snorm,
    b8g8r8a8_uscaled,
    b8g8r8a8_sscaled,
    b8g8r8a8_uint,
    b8g8r8a8_sint,
    b8g8r8a8_srgb,
    a8b8g8r8_unorm_pack32,
    a8b8g8r8_snorm_pack32,
    a8b8g8r8_uscaled_pack32,
    a8b8g8r8_sscaled_pack32,
    a8b8g8r8_uint_pack32,
    a8b8g8r8_sint_pack32,
    a8b8g8r8_srgb_pack32,
    a2r10g10b10_unorm_pack32,
    a2r10g10b10_snorm_pack32,
    a2r10g10b10_uscaled_pack32,
    a2r10g10b10_sscaled_pack32,
    a2r10g10b10_uint_pack32,
    a2r10g10b10_sint_pack32,
    a2b10g10r10_unorm_pack32,
    a2b10g10r10_snorm_pack32,
    a2b10g10r10_uscaled_pack32,
    a2b10g10r10_sscaled_pack32,
    a2b10g10r10_uint_pack32,
    a2b10g10r10_sint_pack32,
    r16_unorm,
    r16_snorm,
    r16_uscaled,
    r16_sscaled,
    r16_uint,
    r16_sint,
    r16_sfloat,
    r16g16_unorm,
    r16g16_snorm,
    r16g16_uscaled,
    r16g16_sscaled,
    r16g16_uint,
    r16g16_sint,
    r16g16_sfloat,
    r16g16b16_unorm,
    r16g16b16_snorm,
    r16g16b16_uscaled,
    r16g16b16_sscaled,
    r16g16b16_uint,
    r16g16b16_sint,
    r16g16b16_sfloat,
    r16g16b16a16_unorm,
    r16g16b16a16_snorm,
    r16g16b16a16_uscaled,
    r16g16b16a16_sscaled,
    r16g16b16a16_uint,
    r16g16b16a16_sint,
    r16g16b16a16_sfloat,
    r32_uint,
    r32_sint,
    r32_sfloat,
    r32g32_uint,
    r32g32_sint,
    r32g32_sfloat,
    r32g32b32_uint,
    r32g32b32_sint,
    r32g32b32_sfloat,
    r32g32b32a32_uint,
    r32g32b32a32_sint,
    r32g32b32a32_sfloat,
    r64_uint,
    r64_sint,
    r64_sfloat,
    r64g64_uint,
    r64g64_sint,
    r64g64_sfloat,
    r64g64b64_uint,
    r64g64b64_sint,
    r64g64b64_sfloat,
    r64g64b64a64_uint,
    r64g64b64a64_sint,
    r64g64b64a64_sfloat,
    b10g11r11_ufloat_pack32,
    e5b9g9r9_ufloat_pack32,
    d16_unorm,
    x8_d24_unorm_pack32,
    d32_sfloat,
    s8_uint,
    d16_unorm_s8_uint,
    d24_unorm_s8_uint,
    d32_sfloat_s8_uint,
    bc1_rgb_unorm_block,
    bc1_rgb_srgb_block,
    bc1_rgba_unorm_block,
    bc1_rgba_srgb_block,
    bc2_unorm_block,
    bc2_srgb_block,
    bc3_unorm_block,
    bc3_srgb_block,
    bc4_unorm_block,
    bc4_snorm_block,
    bc5_unorm_block,
    bc5_snorm_block,
    bc6h_ufloat_block,
    bc6h_sfloat_block,
    bc7_unorm_block,
    bc7_srgb_block,
    etc2_r8g8b8_unorm_block,
    etc2_r8g8b8_srgb_block,
    etc2_r8g8b8a1_unorm_block,
    etc2_r8g8b8a1_srgb_block,
    etc2_r8g8b8a8_unorm_block,
    etc2_r8g8b8a8_srgb_block,
    eac_r11_unorm_block,
    eac_r11_snorm_block,
    eac_r11g11_unorm_block,
    eac_r11g11_snorm_block,
    astc_4x4_unorm_block,
    astc_4x4_srgb_block,
    astc_5x4_unorm_block,
    astc_5x4_srgb_block,
    astc_5x5_unorm_block,
    astc_5x5_srgb_block,
    astc_6x5_unorm_block,
    astc_6x5_srgb_block,
    astc_6x6_unorm_block,
    astc_6x6_srgb_block,
    astc_8x5_unorm_block,
    astc_8x5_srgb_block,
    astc_8x6_unorm_block,
    astc_8x6_srgb_block,
    astc_8x8_unorm_block,
    astc_8x8_srgb_block,
    astc_10x5_unorm_block,
    astc_10x5_srgb_block,
    astc_10x6_unorm_block,
    astc_10x6_srgb_block,
    astc_10x8_unorm_block,
    astc_10x8_srgb_block,
    astc_10x10_unorm_block,
    astc_10x10_srgb_block,
    astc_12x10_unorm_block,
    astc_12x10_srgb_block,
    astc_12x12_unorm_block,
    astc_12x12_srgb_block
};

} // namespace gl

#endif // ANTKEEPER_GL_FORMAT_HPP
