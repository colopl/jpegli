# Copyright (c) the JPEG XL Project Authors.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd

# This file is generated, do not modify by manually.
# Run `tools/scripts/build_cleaner.py --update` to regenerate it.
set(JPEGXL_INTERNAL_BASE_SOURCES
  base/bits.h
  base/byte_order.h
  base/c_callback_support.h
  base/common.h
  base/compiler_specific.h
  base/data_parallel.h
  base/fast_math-inl.h
  base/float.h
  base/include_jpeglib.h
  base/matrix_ops.h
  base/memory_manager.h
  base/os_macros.h
  base/override.h
  base/parallel_runner.h
  base/printf_macros.h
  base/random.h
  base/rational_polynomial-inl.h
  base/rect.h
  base/sanitizer_definitions.h
  base/sanitizers.h
  base/span.h
  base/status.h
  base/types.h
)

set(JPEGXL_INTERNAL_CMS_SOURCES
  cms/cms.h
  cms/cms_interface.h
  cms/color_encoding.h
  cms/color_encoding_cms.h
  cms/color_encoding_internal.h
  cms/jxl_cms.cc
  cms/jxl_cms_internal.h
  cms/opsin_params.h
  cms/tone_mapping-inl.h
  cms/tone_mapping.h
  cms/transfer_functions-inl.h
  cms/transfer_functions.h
)

set(JPEGXL_INTERNAL_CODEC_APNG_SOURCES
  extras/dec/apng.h
  extras/enc/apng.h
)

set(JPEGXL_INTERNAL_CODEC_EXR_SOURCES
  extras/dec/exr.cc
  extras/dec/exr.h
  extras/enc/exr.cc
  extras/enc/exr.h
)

set(JPEGXL_INTERNAL_CODEC_GIF_SOURCES
  extras/dec/gif.cc
  extras/dec/gif.h
)

set(JPEGXL_INTERNAL_CODEC_JPEGLI_SOURCES
  extras/dec/jpegli.cc
  extras/dec/jpegli.h
  extras/enc/jpegli.cc
  extras/enc/jpegli.h
)

set(JPEGXL_INTERNAL_CODEC_JPG_SOURCES
  extras/dec/jpg.cc
  extras/dec/jpg.h
  extras/enc/jpg.cc
  extras/enc/jpg.h
)

set(JPEGXL_INTERNAL_CODEC_NPY_SOURCES
  extras/enc/npy.cc
  extras/enc/npy.h
)

set(JPEGXL_INTERNAL_CODEC_PGX_SOURCES
  extras/dec/pgx.cc
  extras/dec/pgx.h
  extras/enc/pgx.cc
  extras/enc/pgx.h
)

set(JPEGXL_INTERNAL_CODEC_PNM_SOURCES
  extras/dec/pnm.cc
  extras/dec/pnm.h
  extras/enc/pnm.cc
  extras/enc/pnm.h
)

set(JPEGXL_INTERNAL_EXTRAS_FOR_TOOLS_SOURCES
  extras/metrics.cc
  extras/metrics.h
  extras/packed_image_convert.cc
  extras/packed_image_convert.h
)

set(JPEGXL_INTERNAL_EXTRAS_SOURCES
  extras/alpha_blend.cc
  extras/alpha_blend.h
  extras/butteraugli.cc
  extras/butteraugli.h
  extras/codestream_header.h
  extras/convolve-inl.h
  extras/convolve.h
  extras/convolve_separable5.cc
  extras/convolve_slow.cc
  extras/dec/color_description.cc
  extras/dec/color_description.h
  extras/dec/color_hints.cc
  extras/dec/color_hints.h
  extras/dec/decode.cc
  extras/dec/decode.h
  extras/enc/encode.cc
  extras/enc/encode.h
  extras/exif.cc
  extras/exif.h
  extras/image.cc
  extras/image.h
  extras/image_color_transform.cc
  extras/image_color_transform.h
  extras/image_ops.h
  extras/memory_manager_internal.cc
  extras/memory_manager_internal.h
  extras/mmap.cc
  extras/mmap.h
  extras/packed_image.h
  extras/simd_util.cc
  extras/simd_util.h
  extras/size_constraints.h
  extras/time.cc
  extras/time.h
  extras/xyb_transform.cc
  extras/xyb_transform.h
)

set(JPEGXL_INTERNAL_JPEGLI_LIBJPEG_HELPER_FILES
  jpegli/libjpeg_test_util.cc
  jpegli/libjpeg_test_util.h
)

set(JPEGXL_INTERNAL_JPEGLI_SOURCES
  jpegli/adaptive_quantization.cc
  jpegli/adaptive_quantization.h
  jpegli/bit_writer.cc
  jpegli/bit_writer.h
  jpegli/bitstream.cc
  jpegli/bitstream.h
  jpegli/color_quantize.cc
  jpegli/color_quantize.h
  jpegli/color_transform.cc
  jpegli/color_transform.h
  jpegli/common.cc
  jpegli/common.h
  jpegli/common_internal.h
  jpegli/dct-inl.h
  jpegli/decode.cc
  jpegli/decode.h
  jpegli/decode_internal.h
  jpegli/decode_marker.cc
  jpegli/decode_marker.h
  jpegli/decode_scan.cc
  jpegli/decode_scan.h
  jpegli/destination_manager.cc
  jpegli/downsample.cc
  jpegli/downsample.h
  jpegli/encode.cc
  jpegli/encode.h
  jpegli/encode_finish.cc
  jpegli/encode_finish.h
  jpegli/encode_internal.h
  jpegli/encode_streaming.cc
  jpegli/encode_streaming.h
  jpegli/entropy_coding-inl.h
  jpegli/entropy_coding.cc
  jpegli/entropy_coding.h
  jpegli/error.cc
  jpegli/error.h
  jpegli/huffman.cc
  jpegli/huffman.h
  jpegli/idct.cc
  jpegli/idct.h
  jpegli/input.cc
  jpegli/input.h
  jpegli/memory_manager.cc
  jpegli/memory_manager.h
  jpegli/quant.cc
  jpegli/quant.h
  jpegli/render.cc
  jpegli/render.h
  jpegli/simd.cc
  jpegli/simd.h
  jpegli/source_manager.cc
  jpegli/transpose-inl.h
  jpegli/types.h
  jpegli/upsample.cc
  jpegli/upsample.h
)

set(JPEGXL_INTERNAL_JPEGLI_TESTLIB_FILES
  jpegli/fuzztest.h
  jpegli/test_params.h
  jpegli/test_utils-inl.h
  jpegli/test_utils.cc
  jpegli/test_utils.h
)

set(JPEGXL_INTERNAL_JPEGLI_TESTS
  jpegli/decode_api_test.cc
  jpegli/encode_api_test.cc
  jpegli/error_handling_test.cc
  jpegli/input_suspension_test.cc
  jpegli/output_suspension_test.cc
  jpegli/source_manager_test.cc
  jpegli/streaming_test.cc
  jpegli/transcode_api_test.cc
)

set(JPEGXL_INTERNAL_JPEGLI_WRAPPER_SOURCES
  jpegli/libjpeg_wrapper.cc
)

set(JPEGXL_INTERNAL_TESTLIB_FILES
  extras/test_image.cc
  extras/test_image.h
  extras/test_memory_manager.cc
  extras/test_memory_manager.h
  extras/test_utils.cc
  extras/test_utils.h
  threads/test_utils.h
)

set(JPEGXL_INTERNAL_TESTS
  cms/tone_mapping_test.cc
  cms/transfer_functions_test.cc
  extras/butteraugli_test.cc
  extras/codec_test.cc
  extras/dec/color_description_test.cc
  extras/jpegli_test.cc
  threads/thread_parallel_runner_test.cc
)

set(JPEGXL_INTERNAL_THREADS_SOURCES
  threads/thread_parallel_runner.cc
  threads/thread_parallel_runner.h
  threads/thread_parallel_runner_cxx.h
  threads/thread_parallel_runner_internal.cc
  threads/thread_parallel_runner_internal.h
)
