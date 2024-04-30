// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <jxl/cms.h>
#include <stdio.h>

#include <utility>

#include "lib/jxl/base/compiler_specific.h"
#include "lib/jxl/base/data_parallel.h"
#include "lib/jxl/base/printf_macros.h"
#include "lib/jxl/base/status.h"
#include "lib/jxl/codec_in_out.h"
#include "lib/jxl/color_encoding_internal.h"
#include "lib/jxl/enc_xyb.h"
#include "lib/jxl/image.h"
#include "lib/jxl/image_bundle.h"

namespace jpegxl {
namespace tools {
namespace {

using ::jxl::CodecInOut;
using ::jxl::ColorEncoding;
using ::jxl::Image3F;
using ::jxl::ImageBundle;
using ::jxl::Status;
using ::jxl::ThreadPool;

Status PrintXybRange() {
  JXL_ASSIGN_OR_RETURN(Image3F linear, Image3F::Create(1u << 16, 257));
  for (int b = 0; b < 256; ++b) {
    float* JXL_RESTRICT row0 = linear.PlaneRow(0, b + 1);
    float* JXL_RESTRICT row1 = linear.PlaneRow(1, b + 1);
    float* JXL_RESTRICT row2 = linear.PlaneRow(2, b + 1);
    for (int r = 0; r < 256; ++r) {
      for (int g = 0; g < 256; ++g) {
        const int x = (r << 8) + g;
        row0[x] = r;
        row1[x] = g;
        row2[x] = b;
      }
    }
  }
  CodecInOut io;
  io.metadata.m.SetUintSamples(8);
  io.metadata.m.color_encoding = ColorEncoding::LinearSRGB();
  io.SetFromImage(std::move(linear), io.metadata.m.color_encoding);
  const ImageBundle& ib = io.Main();
  ThreadPool* null_pool = nullptr;
  JXL_ASSIGN_OR_RETURN(Image3F opsin, Image3F::Create(ib.xsize(), ib.ysize()));
  (void)jxl::ToXYB(ib, null_pool, &opsin, *JxlGetDefaultCms());
  for (size_t c = 0; c < 3; ++c) {
    float minval = 1e10f;
    float maxval = -1e10f;
    int rgb_min = 0;
    int rgb_max = 0;
    for (int b = 0; b < 256; ++b) {
      const float* JXL_RESTRICT row = opsin.PlaneRow(c, b);
      for (int r = 0; r < 256; ++r) {
        for (int g = 0; g < 256; ++g) {
          float val = row[(r << 8) + g];
          if (val < minval) {
            minval = val;
            rgb_min = (r << 16) + (g << 8) + b;
          }
          if (val > maxval) {
            maxval = val;
            rgb_max = (r << 16) + (g << 8) + b;
          }
        }
      }
    }
    printf("Opsin image plane %" PRIuS
           " range: [%8.4f, %8.4f] "
           "center: %.12f, range: %.12f (RGBmin=%06x, RGBmax=%06x)\n",
           c, minval, maxval, 0.5 * (minval + maxval), 0.5 * (maxval - minval),
           rgb_min, rgb_max);
    // Ensure our constants are at least as wide as those obtained from sRGB.
  }
  return true;
}

}  // namespace
}  // namespace tools
}  // namespace jpegxl

int main() { JXL_CHECK(jpegxl::tools::PrintXybRange()); }
