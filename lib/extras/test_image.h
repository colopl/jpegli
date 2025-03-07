// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef LIB_EXTRAS_TEST_IMAGE_H_
#define LIB_EXTRAS_TEST_IMAGE_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "lib/base/status.h"
#include "lib/base/types.h"
#include "lib/extras/codestream_header.h"
#include "lib/extras/packed_image.h"

namespace jxl {
namespace test {

// Returns a test image with some autogenerated pixel content, using 16 bits per
// channel, big endian order, 1 to 4 channels
// The seed parameter allows to create images with different pixel content.
std::vector<uint8_t> GetSomeTestImage(size_t xsize, size_t ysize,
                                      size_t num_channels, uint16_t seed);

class TestImage {
 public:
  TestImage();

  extras::PackedPixelFile& ppf() { return ppf_; }

  Status DecodeFromBytes(const std::vector<uint8_t>& bytes);

  TestImage& ClearMetadata();

  Status SetDimensions(size_t xsize, size_t ysize);

  Status SetChannels(size_t num_channels);

  // Sets the same bit depth on color, alpha and all extra channels.
  TestImage& SetAllBitDepths(uint32_t bits_per_sample,
                             uint32_t exponent_bits_per_sample = 0);

  TestImage& SetDataType(JxlDataType data_type);

  TestImage& SetEndianness(JxlEndianness endianness);

  TestImage& SetRowAlignment(size_t align);

  Status SetColorEncoding(const std::string& description);

  Status CoalesceGIFAnimationWithAlpha();

  class Frame {
   public:
    Frame(TestImage* parent, bool is_preview, size_t index);

    void ZeroFill();
    void RandomFill(uint16_t seed = 177);

    Status SetValue(size_t y, size_t x, size_t c, float val);

   private:
    extras::PackedPixelFile& ppf() const { return parent_->ppf(); }

    extras::PackedFrame& frame() {
      return is_preview_ ? *ppf().preview_frame : ppf().frames[index_];
    }

    TestImage* parent_;
    bool is_preview_;
    size_t index_;
  };

  StatusOr<Frame> AddFrame();

 private:
  extras::PackedPixelFile ppf_;
  JxlPixelFormat format_ = {3, JXL_TYPE_UINT8, JXL_LITTLE_ENDIAN, 0};

  static void CropLayerInfo(size_t xsize, size_t ysize, JxlLayerInfo* info);

  static void CropImage(size_t xsize, size_t ysize, extras::PackedImage* image);

  static JxlDataType DefaultDataType(const JxlBasicInfo& info);
};

}  // namespace test
}  // namespace jxl

#endif  // LIB_EXTRAS_TEST_IMAGE_H_
