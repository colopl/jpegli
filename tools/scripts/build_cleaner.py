#!/usr/bin/env python3
# Copyright (c) the JPEG XL Project Authors.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd


"""build_cleaner.py: Update build files.

This tool keeps certain parts of the build files up to date.
"""

import argparse
import locale
import os
import re
import subprocess
import sys
import tempfile

COPYRIGHT = [
  "Copyright (c) the JPEG XL Project Authors.",
  "",
  "Use of this source code is governed by a BSD-style",
  "license that can be found in the LICENSE file or at",
  "https://developers.google.com/open-source/licenses/bsd"
]

DOC = [
  "This file is generated, do not modify by manually.",
  "Run `tools/scripts/build_cleaner.py --update` to regenerate it."
]

def RepoFiles(src_dir):
  """Return the list of files from the source git repository"""
  git_bin = os.environ.get('GIT_BIN', 'git')
  files = subprocess.check_output([git_bin, '-C', src_dir, 'ls-files'])
  ret = files.decode(locale.getpreferredencoding()).splitlines()
  ret.sort()
  return ret


def Check(condition, msg):
  if not condition:
    print(msg)
    sys.exit(2)


def ContainsFn(*parts):
  return lambda path: any(part in path for part in parts)


def HasPrefixFn(*prefixes):
  return lambda path: any(path.startswith(prefix) for prefix in prefixes)


def HasSuffixFn(*suffixes):
  return lambda path: any(path.endswith(suffix) for suffix in suffixes)


def Filter(src, fn):
  yes_list = []
  no_list = []
  for item in src:
    (yes_list if fn(item) else no_list).append(item)
  return yes_list, no_list


def SplitLibFiles(repo_files):
  """Splits the library files into the different groups."""

  srcs_base = 'lib/'
  srcs, _ = Filter(repo_files, HasPrefixFn(srcs_base))
  srcs = [path[len(srcs_base):] for path in srcs]
  srcs, _ = Filter(srcs, HasSuffixFn('.cc', '.h', '.ui'))
  srcs.sort()

  # Let's keep Jpegli sources a bit separate for a while.
  jpegli_srcs, srcs = Filter(srcs, HasPrefixFn('jpegli'))
  # This stub compilation unit is manually referenced in CMake buildfile.
  _, srcs = Filter(srcs, HasSuffixFn('nothing.cc'))

  # First pick files scattered across directories.
  tests, srcs = Filter(srcs, HasSuffixFn('_test.cc'))
  jpegli_tests, jpegli_srcs = Filter(jpegli_srcs, HasSuffixFn('_test.cc'))
  # TODO(eustas): move to separate list?
  _, srcs = Filter(srcs, ContainsFn('testing.h'))
  _, jpegli_srcs = Filter(jpegli_srcs, ContainsFn('testing.h'))
  testlib_files, srcs = Filter(srcs, ContainsFn('test'))
  jpegli_testlib_files, jpegli_srcs = Filter(jpegli_srcs, ContainsFn('test'))
  jpegli_libjpeg_helper_files, jpegli_testlib_files = Filter(
    jpegli_testlib_files, ContainsFn('libjpeg_test_util'))

  extras_sources, srcs = Filter(srcs, HasPrefixFn('extras/'))
  threads_sources, srcs = Filter(srcs, HasPrefixFn('threads/'))

  base_sources, srcs = Filter(srcs, HasPrefixFn('base/'))
  cms_sources, srcs = Filter(srcs, HasPrefixFn('cms/'))

  Check(len(srcs) == 0, 'Orphan source files: ' + str(srcs))

  jpegli_wrapper_sources, jpegli_srcs = Filter(
      jpegli_srcs, HasSuffixFn('libjpeg_wrapper.cc'))
  jpegli_sources = jpegli_srcs

  codec_names = ['apng', 'exr', 'gif', 'jpegli', 'jpg', 'npy', 'pgx',
    'pnm']
  codecs = {}
  for codec in codec_names:
    codec_sources, extras_sources = Filter(extras_sources, HasPrefixFn(
      f'extras/dec/{codec}', f'extras/enc/{codec}'))
    codecs[f'codec_{codec}_sources'] = codec_sources

  # TODO(eustas): move to separate folder?
  extras_for_tools_sources, extras_sources = Filter(extras_sources, ContainsFn(
    '/codec', '/hlg', '/metrics', '/packed_image_convert', '/render_hdr',
    '/tone_mapping'))


  return codecs | {'base_sources': base_sources,
    'cms_sources': cms_sources,
    'extras_for_tools_sources': extras_for_tools_sources,
    'extras_sources': extras_sources,
    'jpegli_sources': jpegli_sources,
    'jpegli_testlib_files': jpegli_testlib_files,
    'jpegli_libjpeg_helper_files': jpegli_libjpeg_helper_files,
    'jpegli_tests': jpegli_tests,
    'jpegli_wrapper_sources' : jpegli_wrapper_sources,
    'testlib_files': testlib_files,
    'tests': tests,
    'threads_sources': threads_sources,
  }


def MaybeUpdateFile(args, filename, new_text):
  """Optionally replace file with new contents.

  If args.update is set, it will update the file with the new contents,
  otherwise it will return True when no changes were needed.
  """
  filepath = os.path.join(args.src_dir, filename)
  with open(filepath, 'r') as f:
    src_text = f.read()

  if new_text == src_text:
    return True

  if args.update:
    print('Updating %s' % filename)
    with open(filepath, 'w') as f:
      f.write(new_text)
    return True
  else:
    prefix = os.path.basename(filename)
    with tempfile.NamedTemporaryFile(mode='w', prefix=prefix) as new_file:
      new_file.write(new_text)
      new_file.flush()
      subprocess.call(['diff', '-u', filepath, '--label', 'a/' + filename,
        new_file.name, '--label', 'b/' + filename])
    return False


def FormatList(items, prefix, suffix):
  return ''.join(f'{prefix}{item}{suffix}\n' for item in items)


def FormatGniVar(name, var):
  if type(var) is list:
    contents = FormatList(var, '    "', '",')
    return f'{name} = [\n{contents}]\n'
  else:  # TODO(eustas): do we need scalar strings?
    return f'{name} = {var}\n'


def FormatCMakeVar(name, var):
  if type(var) is list:
    contents = FormatList(var, '  ', '')
    return f'set({name}\n{contents})\n'
  else:  # TODO(eustas): do we need scalar strings?
    return f'set({name} {var})\n'


def GetJpegLibVersion(src_dir):
  with open(os.path.join(src_dir, 'CMakeLists.txt'), 'r') as f:
    cmake_text = f.read()
    m = re.search(r'set\(JPEGLI_LIBJPEG_LIBRARY_SOVERSION "([0-9]+)"',
                  cmake_text)
    version = m.group(1)
    if len(version) == 1:
      version += "0"
    return version

def ToHashComment(lines):
  return [("# " + line).rstrip() for line in lines]
def ToDocstringComment(lines):
  return ["\"\"\""] + lines + ["\"\"\""]

def BuildCleaner(args):
  repo_files = RepoFiles(args.src_dir)

  with open(os.path.join(args.src_dir, 'lib/CMakeLists.txt'), 'r') as f:
    cmake_text = f.read()
  version = {'major_version': '', 'minor_version': '', 'patch_version': ''}
  for var in version.keys():
    cmake_var = f'JPEGXL_{var.upper()}'
    # TODO(eustas): use `cmake -L`
    # Regexp:
    #   set(_varname_ _capture_decimal_)
    match = re.search(r'set\(' + cmake_var + r' ([0-9]+)\)', cmake_text)
    version[var] = match.group(1)

  version['jpegli_lib_version'] = GetJpegLibVersion(args.src_dir)

  lists = SplitLibFiles(repo_files)

  cmake_chunks = ToHashComment(COPYRIGHT) + [""] + ToHashComment(DOC)
  cmake_parts = lists
  for var in sorted(cmake_parts):
    cmake_chunks.append(FormatCMakeVar(
        'JPEGXL_INTERNAL_' + var.upper(), cmake_parts[var]))

  gni_bzl_parts = version | lists
  gni_bzl_chunks = []
  for var in sorted(gni_bzl_parts):
    gni_bzl_chunks.append(FormatGniVar('libjxl_' + var, gni_bzl_parts[var]))

  bzl_chunks = ToHashComment(COPYRIGHT) + [""] + \
      ToDocstringComment(DOC) + [""] + gni_bzl_chunks
  gni_chunks = ToHashComment(COPYRIGHT) + [""] + \
      ToHashComment(DOC) + [""] + gni_bzl_chunks

  okay = [
    MaybeUpdateFile(args, 'lib/jxl_lists.bzl', '\n'.join(bzl_chunks)),
    MaybeUpdateFile(args, 'lib/jxl_lists.cmake', '\n'.join(cmake_chunks)),
    MaybeUpdateFile(args, 'lib/lib.gni', '\n'.join(gni_chunks)),
  ]
  return all(okay)


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('--src-dir',
    default=os.path.realpath(os.path.join( os.path.dirname(__file__), '../..')),
    help='path to the build directory')
  parser.add_argument('--update', default=False, action='store_true',
    help='update the build files instead of only checking')
  args = parser.parse_args()
  Check(BuildCleaner(args), 'Build files need update.')


if __name__ == '__main__':
  main()
