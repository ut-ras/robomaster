#!/usr/bin/python3
#
# Copyright (c) 2020-2021 UT Robomaster
#
# This file is part of template-project.
#
# template-project is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# template-project is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with template-project.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import subprocess
import shutil
import os

FILE_EXTENSIONS_TO_FORMAT = ['.c', '.cc', '.cpp', '.h', '.hh', '.hpp']

def parse_args():
    parser = argparse.ArgumentParser(description='Run clang-format on a list of specified directories.')
    parser.add_argument('-e', '--exe', type=str,
                        help='a clang-format executable (can be in your PATH) to use when running clang-format')
    parser.add_argument('-d', '--dirs', type=str, nargs='+', required=True,
                        help='a list of directories that will be formatted, relative to the current working directory')
    args = parser.parse_args()

    clang_fmt = args.exe
    clang_fmt_dirs = args.dirs

    if clang_fmt == None:
        # find default clang-format exe if not passed in
        clang_fmt = shutil.which('clang-format')

    # check passed in clang-format or clang-format found when running `which`
    if shutil.which(clang_fmt) is None:
        raise ValueError(f'clang-format executable argument (--exe) invalid, cannot find {clang_fmt}!')

    return clang_fmt, clang_fmt_dirs

def main():
    clang_fmt, clang_fmt_dirs = parse_args()

    for d in clang_fmt_dirs:
        for dirpath, _, filenames in os.walk(d):
            # only process c/c++ file extensions
            filenames = list(filter(lambda file: os.path.splitext(file)[1] in FILE_EXTENSIONS_TO_FORMAT, filenames))
            # append directory path to filename
            filenames = [os.path.join(dirpath, file) for file in filenames]
            try:
                for file in filenames:
                    subprocess.run([clang_fmt, '-i', file], check=True)
            except subprocess.CalledProcessError as e:
                print(e)
                exit(1)

if __name__ == '__main__':
    main()
