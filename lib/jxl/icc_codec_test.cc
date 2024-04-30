// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "lib/jxl/icc_codec.h"

#include <cstdint>
#include <string>
#include <vector>

#include "lib/jxl/base/span.h"
#include "lib/jxl/color_encoding_internal.h"
#include "lib/jxl/enc_icc_codec.h"
#include "lib/jxl/test_utils.h"
#include "lib/jxl/testing.h"

namespace jxl {
namespace {

void TestProfile(const IccBytes& icc) {
  BitWriter writer;
  ASSERT_TRUE(WriteICC(icc, &writer, 0, nullptr));
  writer.ZeroPadToByte();
  std::vector<uint8_t> dec;
  BitReader reader(writer.GetSpan());
  ASSERT_TRUE(test::ReadICC(&reader, &dec));
  ASSERT_TRUE(reader.Close());
  EXPECT_EQ(icc.size(), dec.size());
  if (icc.size() == dec.size()) {
    for (size_t i = 0; i < icc.size(); i++) {
      EXPECT_EQ(icc[i], dec[i]);
      if (icc[i] != dec[i]) break;  // One output is enough
    }
  }
}

void TestProfile(const std::string& icc) {
  IccBytes data;
  Bytes(icc).AppendTo(data);
  TestProfile(data);
}

// Valid profile from one of the images output by the decoder.
const unsigned char kTestProfile[] = {
    0x00, 0x00, 0x03, 0x80, 0x6c, 0x63, 0x6d, 0x73, 0x04, 0x30, 0x00, 0x00,
    0x6d, 0x6e, 0x74, 0x72, 0x52, 0x47, 0x42, 0x20, 0x58, 0x59, 0x5a, 0x20,
    0x07, 0xe3, 0x00, 0x04, 0x00, 0x1d, 0x00, 0x0f, 0x00, 0x32, 0x00, 0x2e,
    0x61, 0x63, 0x73, 0x70, 0x41, 0x50, 0x50, 0x4c, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xf6, 0xd6,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x2d, 0x6c, 0x63, 0x6d, 0x73,
    0x5f, 0x07, 0x0d, 0x3e, 0x4d, 0x32, 0xf2, 0x6e, 0x5d, 0x77, 0x26, 0xcc,
    0x23, 0xb0, 0x6a, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d,
    0x64, 0x65, 0x73, 0x63, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x42,
    0x63, 0x70, 0x72, 0x74, 0x00, 0x00, 0x01, 0x64, 0x00, 0x00, 0x01, 0x00,
    0x77, 0x74, 0x70, 0x74, 0x00, 0x00, 0x02, 0x64, 0x00, 0x00, 0x00, 0x14,
    0x63, 0x68, 0x61, 0x64, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0x00, 0x2c,
    0x72, 0x58, 0x59, 0x5a, 0x00, 0x00, 0x02, 0xa4, 0x00, 0x00, 0x00, 0x14,
    0x62, 0x58, 0x59, 0x5a, 0x00, 0x00, 0x02, 0xb8, 0x00, 0x00, 0x00, 0x14,
    0x67, 0x58, 0x59, 0x5a, 0x00, 0x00, 0x02, 0xcc, 0x00, 0x00, 0x00, 0x14,
    0x72, 0x54, 0x52, 0x43, 0x00, 0x00, 0x02, 0xe0, 0x00, 0x00, 0x00, 0x20,
    0x67, 0x54, 0x52, 0x43, 0x00, 0x00, 0x02, 0xe0, 0x00, 0x00, 0x00, 0x20,
    0x62, 0x54, 0x52, 0x43, 0x00, 0x00, 0x02, 0xe0, 0x00, 0x00, 0x00, 0x20,
    0x63, 0x68, 0x72, 0x6d, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x24,
    0x64, 0x6d, 0x6e, 0x64, 0x00, 0x00, 0x03, 0x24, 0x00, 0x00, 0x00, 0x28,
    0x64, 0x6d, 0x64, 0x64, 0x00, 0x00, 0x03, 0x4c, 0x00, 0x00, 0x00, 0x32,
    0x6d, 0x6c, 0x75, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x0c, 0x65, 0x6e, 0x55, 0x53, 0x00, 0x00, 0x00, 0x26,
    0x00, 0x00, 0x00, 0x1c, 0x00, 0x52, 0x00, 0x47, 0x00, 0x42, 0x00, 0x5f,
    0x00, 0x44, 0x00, 0x36, 0x00, 0x35, 0x00, 0x5f, 0x00, 0x53, 0x00, 0x52,
    0x00, 0x47, 0x00, 0x5f, 0x00, 0x52, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x5f,
    0x00, 0x37, 0x00, 0x30, 0x00, 0x39, 0x00, 0x00, 0x6d, 0x6c, 0x75, 0x63,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c,
    0x65, 0x6e, 0x55, 0x53, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x1c,
    0x00, 0x43, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x79, 0x00, 0x72, 0x00, 0x69,
    0x00, 0x67, 0x00, 0x68, 0x00, 0x74, 0x00, 0x20, 0x00, 0x32, 0x00, 0x30,
    0x00, 0x31, 0x00, 0x38, 0x00, 0x20, 0x00, 0x47, 0x00, 0x6f, 0x00, 0x6f,
    0x00, 0x67, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x4c,
    0x00, 0x43, 0x00, 0x2c, 0x00, 0x20, 0x00, 0x43, 0x00, 0x43, 0x00, 0x2d,
    0x00, 0x42, 0x00, 0x59, 0x00, 0x2d, 0x00, 0x53, 0x00, 0x41, 0x00, 0x20,
    0x00, 0x33, 0x00, 0x2e, 0x00, 0x30, 0x00, 0x20, 0x00, 0x55, 0x00, 0x6e,
    0x00, 0x70, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x74, 0x00, 0x65, 0x00, 0x64,
    0x00, 0x20, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e,
    0x00, 0x73, 0x00, 0x65, 0x00, 0x28, 0x00, 0x68, 0x00, 0x74, 0x00, 0x74,
    0x00, 0x70, 0x00, 0x73, 0x00, 0x3a, 0x00, 0x2f, 0x00, 0x2f, 0x00, 0x63,
    0x00, 0x72, 0x00, 0x65, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x76,
    0x00, 0x65, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x6d, 0x00, 0x6f,
    0x00, 0x6e, 0x00, 0x73, 0x00, 0x2e, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x67,
    0x00, 0x2f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e,
    0x00, 0x73, 0x00, 0x65, 0x00, 0x73, 0x00, 0x2f, 0x00, 0x62, 0x00, 0x79,
    0x00, 0x2d, 0x00, 0x73, 0x00, 0x61, 0x00, 0x2f, 0x00, 0x33, 0x00, 0x2e,
    0x00, 0x30, 0x00, 0x2f, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x67, 0x00, 0x61,
    0x00, 0x6c, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x29,
    0x58, 0x59, 0x5a, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xd6,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x2d, 0x73, 0x66, 0x33, 0x32,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0c, 0x42, 0x00, 0x00, 0x05, 0xde,
    0xff, 0xff, 0xf3, 0x25, 0x00, 0x00, 0x07, 0x93, 0x00, 0x00, 0xfd, 0x90,
    0xff, 0xff, 0xfb, 0xa1, 0xff, 0xff, 0xfd, 0xa2, 0x00, 0x00, 0x03, 0xdc,
    0x00, 0x00, 0xc0, 0x6e, 0x58, 0x59, 0x5a, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x6f, 0xa0, 0x00, 0x00, 0x38, 0xf5, 0x00, 0x00, 0x03, 0x90,
    0x58, 0x59, 0x5a, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x9f,
    0x00, 0x00, 0x0f, 0x84, 0x00, 0x00, 0xb6, 0xc4, 0x58, 0x59, 0x5a, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x97, 0x00, 0x00, 0xb7, 0x87,
    0x00, 0x00, 0x18, 0xd9, 0x70, 0x61, 0x72, 0x61, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x38, 0xe4, 0x00, 0x00, 0xe8, 0xf0,
    0x00, 0x00, 0x17, 0x10, 0x00, 0x00, 0x38, 0xe4, 0x00, 0x00, 0x14, 0xbc,
    0x63, 0x68, 0x72, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0xa3, 0xd7, 0x00, 0x00, 0x54, 0x7c, 0x00, 0x00, 0x4c, 0xcd,
    0x00, 0x00, 0x99, 0x9a, 0x00, 0x00, 0x26, 0x67, 0x00, 0x00, 0x0f, 0x5c,
    0x6d, 0x6c, 0x75, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x0c, 0x65, 0x6e, 0x55, 0x53, 0x00, 0x00, 0x00, 0x0c,
    0x00, 0x00, 0x00, 0x1c, 0x00, 0x47, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x67,
    0x00, 0x6c, 0x00, 0x65, 0x6d, 0x6c, 0x75, 0x63, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x65, 0x6e, 0x55, 0x53,
    0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x49, 0x00, 0x6d,
    0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x20, 0x00, 0x63, 0x00, 0x6f,
    0x00, 0x64, 0x00, 0x65, 0x00, 0x63, 0x00, 0x00,
};

}  // namespace

TEST(IccCodecTest, Icc) {
  // Empty string cannot be tested, encoder checks against writing it.
  TestProfile("a");
  TestProfile("ab");
  TestProfile("aaaa");

  {
    // Exactly the ICC header size
    IccBytes profile(128);
    for (size_t i = 0; i < 128; i++) {
      profile[i] = 0;
    }
    TestProfile(profile);
  }

  {
    IccBytes profile;
    Bytes(kTestProfile, sizeof(kTestProfile)).AppendTo(profile);
    TestProfile(profile);
  }

  // Test substrings of full profile
  {
    IccBytes profile;
    for (size_t i = 0; i <= 256; i++) {
      profile.push_back(kTestProfile[i]);
      TestProfile(profile);
    }
  }
}

// kTestProfile after encoding with the ICC codec
static const unsigned char kEncodedTestProfile[] = {
    0x1f, 0x8b, 0x1,  0x13, 0x10, 0x0,  0x0,  0x0,  0x20, 0x4c, 0xcc, 0x3,
    0xe7, 0xa0, 0xa5, 0xa2, 0x90, 0xa4, 0x27, 0xe8, 0x79, 0x1d, 0xe3, 0x26,
    0x57, 0x54, 0xef, 0x0,  0xe8, 0x97, 0x2,  0xce, 0xa1, 0xd7, 0x85, 0x16,
    0xb4, 0x29, 0x94, 0x58, 0xf2, 0x56, 0xc0, 0x76, 0xea, 0x23, 0xec, 0x7c,
    0x73, 0x51, 0x41, 0x40, 0x23, 0x21, 0x95, 0x4,  0x75, 0x12, 0xc9, 0xcc,
    0x16, 0xbd, 0xb6, 0x99, 0xad, 0xf8, 0x75, 0x35, 0xb6, 0x42, 0xae, 0xae,
    0xae, 0x86, 0x56, 0xf8, 0xcc, 0x16, 0x30, 0xb3, 0x45, 0xad, 0xd,  0x40,
    0xd6, 0xd1, 0xd6, 0x99, 0x40, 0xbe, 0xe2, 0xdc, 0x31, 0x7,  0xa6, 0xb9,
    0x27, 0x92, 0x38, 0x0,  0x3,  0x5e, 0x2c, 0xbe, 0xe6, 0xfb, 0x19, 0xbf,
    0xf3, 0x6d, 0xbc, 0x4d, 0x64, 0xe5, 0xba, 0x76, 0xde, 0x31, 0x65, 0x66,
    0x14, 0xa6, 0x3a, 0xc5, 0x8f, 0xb1, 0xb4, 0xba, 0x1f, 0xb1, 0xb8, 0xd4,
    0x75, 0xba, 0x18, 0x86, 0x95, 0x3c, 0x26, 0xf6, 0x25, 0x62, 0x53, 0xfd,
    0x9c, 0x94, 0x76, 0xf6, 0x95, 0x2c, 0xb1, 0xfd, 0xdc, 0xc0, 0xe4, 0x3f,
    0xb3, 0xff, 0x67, 0xde, 0xd5, 0x94, 0xcc, 0xb0, 0x83, 0x2f, 0x28, 0x93,
    0x92, 0x3,  0xa1, 0x41, 0x64, 0x60, 0x62, 0x70, 0x80, 0x87, 0xaf, 0xe7,
    0x60, 0x4a, 0x20, 0x23, 0xb3, 0x11, 0x7,  0x38, 0x38, 0xd4, 0xa,  0x66,
    0xb5, 0x93, 0x41, 0x90, 0x19, 0x17, 0x18, 0x60, 0xa5, 0xb,  0x7a, 0x24,
    0xaa, 0x20, 0x81, 0xac, 0xa9, 0xa1, 0x70, 0xa6, 0x12, 0x8a, 0x4a, 0xa3,
    0xa0, 0xf9, 0x9a, 0x97, 0xe7, 0xa8, 0xac, 0x8,  0xa8, 0xc4, 0x2a, 0x86,
    0xa7, 0x69, 0x1e, 0x67, 0xe6, 0xbe, 0xa4, 0xd3, 0xff, 0x91, 0x61, 0xf6,
    0x8a, 0xe6, 0xb5, 0xb3, 0x61, 0x9f, 0x19, 0x17, 0x98, 0x27, 0x6b, 0xe9,
    0x8,  0x98, 0xe1, 0x21, 0x4a, 0x9,  0xb5, 0xd7, 0xca, 0xfa, 0x94, 0xd0,
    0x69, 0x1a, 0xeb, 0x52, 0x1,  0x4e, 0xf5, 0xf6, 0xdf, 0x7f, 0xe7, 0x29,
    0x70, 0xee, 0x4,  0xda, 0x2f, 0xa4, 0xff, 0xfe, 0xbb, 0x6f, 0xa8, 0xff,
    0xfe, 0xdb, 0xaf, 0x8,  0xf6, 0x72, 0xa1, 0x40, 0x5d, 0xf0, 0x2d, 0x8,
    0x82, 0x5b, 0x87, 0xbd, 0x10, 0x8,  0xe9, 0x7,  0xee, 0x4b, 0x80, 0xda,
    0x4a, 0x4,  0xc5, 0x5e, 0xa0, 0xb7, 0x1e, 0x60, 0xb0, 0x59, 0x76, 0x60,
    0xb,  0x2e, 0x19, 0x8a, 0x2e, 0x1c, 0xe6, 0x6,  0x20, 0xb8, 0x64, 0x18,
    0x2a, 0xcf, 0x51, 0x94, 0xd4, 0xee, 0xc3, 0xfe, 0x39, 0x74, 0xd4, 0x2b,
    0x48, 0xc9, 0x83, 0x4c, 0x9b, 0xd0, 0x4c, 0x35, 0x10, 0xe3, 0x9,  0xf7,
    0x72, 0xf0, 0x7a, 0xe,  0xbf, 0x7d, 0x36, 0x2e, 0x19, 0x7e, 0x3f, 0xc,
    0xf7, 0x93, 0xe7, 0xf4, 0x1d, 0x32, 0xc6, 0xb0, 0x89, 0xad, 0xe0, 0x28,
    0xc1, 0xa7, 0x59, 0xe3, 0x0,
};

// Tests that the decoded kEncodedTestProfile matches kTestProfile.
TEST(IccCodecTest, EncodedIccProfile) {
  jxl::BitReader reader(
      jxl::Bytes(kEncodedTestProfile, sizeof(kEncodedTestProfile)));
  std::vector<uint8_t> dec;
  ASSERT_TRUE(test::ReadICC(&reader, &dec));
  ASSERT_TRUE(reader.Close());
  EXPECT_EQ(sizeof(kTestProfile), dec.size());
  if (sizeof(kTestProfile) == dec.size()) {
    for (size_t i = 0; i < dec.size(); i++) {
      EXPECT_EQ(kTestProfile[i], dec[i]);
      if (kTestProfile[i] != dec[i]) break;  // One output is enough
    }
  }
}

}  // namespace jxl
