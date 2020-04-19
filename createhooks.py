#!/usr/bin/env python
# Copyright (C) 2019 Adrian Carpenter
# 
# Simple GIT commit-msg hook that conforms to the conventional commits style.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import shutil
import os

try:
	shutil.copy2('tools/githooks/post-commit', '.git/hooks/')
except IOError as io_err:
    os.makedirs(os.path.dirname('.git/hooks/'))
    shutil.copy('tools/githooks/post-commit', '.git/hooks/')