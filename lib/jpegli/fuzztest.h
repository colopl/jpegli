// Copyright (c) the JPEG XL Project Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef LIB_JPEGLI_FUZZTEST_H_
#define LIB_JPEGLI_FUZZTEST_H_

#include "lib/base/compiler_specific.h"

#if !defined(FUZZ_TEST)
struct FuzzTestSink {
  template <typename F>
  FuzzTestSink WithSeeds(F /*f*/) {
    return *this;
  }
};
#define FUZZ_TEST(A, B) \
  const JXL_MAYBE_UNUSED FuzzTestSink unused##A##B = FuzzTestSink()
#endif

#endif  // LIB_JPEGLI_FUZZTEST_H_
