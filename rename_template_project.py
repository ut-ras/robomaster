#! /usr/bin/python3
#
# Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
#
# This file is part of taproot-template-project.
#
# taproot-template-project is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# taproot-template-project is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with taproot-template-project.  If not, see <https://www.gnu.org/licenses/>.

'''
This script replaces all instance of 'taproot-template-project' with a user specified
project name (<new-repo-name>) and replaces 'taproot-template' with
'<new-repo-name>-project'. It is assumed that the <new-repo-name> is the name of
the repository created that the taproot-template-project was copied to.

Also updates the copyright license header with the team name, which you must also
specify.

This script also renames the `template-project` directory with
'<new-repo-name>-project.'

If, for example, you run this script and specify a <new-repo-name> as 'cool-team',
the repository you created will have the following directory structure

|-- cool-team
|   |-- cool-team-project
|   |   |-- ...
|   |-- ...
'''

import os
import fileinput
import sys

DIRS_TO_EXCLUDE = ['taproot', 'taproot-scripts', '.git', 'template-project/taproot']
DIRS_TO_EXCLUDE = [os.path.normpath(p) for p in DIRS_TO_EXCLUDE]

if len(sys.argv) != 3:
    print('Usage: python3 rename_template_project.py <new-repo-name> <team-name>')
    exit(1)

new_repo_name = sys.argv[1]
team_name = sys.argv[2]

# remove stuff from the README that shouldn't be there
with fileinput.FileInput('README.md', inplace=True, backup='.bak') as f:
    removing_section = False
    for line in f:
        if '<!-- Start sections that may be removed after forking this repository -->' in line:
            removing_section = True

        if not removing_section:
            print(line, end='')

        if removing_section and '<!-- End sections that may be removed after forking this repository -->' in line:
            removing_section = False

    os.remove('README.md.bak')

# rename all files that have template-project or taproot-template-project in them
for root, dirs, files in os.walk('.'):
    dirs[:] = [d for d in dirs if os.path.normpath(os.path.join(root, d)) not in DIRS_TO_EXCLUDE]

    files = [os.path.join(root, f) for f in files]

    for file in files:
        if 'rename_template_project.py' in file:
            continue

        with fileinput.FileInput(file, inplace=True, backup='.bak') as f:
            for line in f:
                if 'Advanced Robotics at the University of Washington <robomstr@uw.edu>' in line:
                    print(line.replace('Advanced Robotics at the University of Washington <robomstr@uw.edu>', team_name), end='')
                else:
                    print(line.replace('taproot-template-project', new_repo_name).replace('template-project', f'{new_repo_name}-project'), end='')

        os.remove(f'{file}.bak')

# rename the template project directory
os.rename('template-project', f'{new_repo_name}-project')