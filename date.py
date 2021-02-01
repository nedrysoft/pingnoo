#!/usr/bin/env python3

#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 27/03/2020.
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

import os
import sys
import datetime

if len(sys.argv) == 2:
    filename = sys.argv[1]

    datePipe = os.popen(f'git log --format=%aD {filename} | tail -1')
    authorPipe = os.popen(f'git log --format=%aN {filename} | tail -1')

    author = authorPipe.read().rstrip()

    dateString = datetime.datetime.strptime(datePipe.read().rstrip(), "%a, %d %b %Y %H:%M:%S %z").strftime("%d/%m/%Y")

    print(f' * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)')
    print(f' *')
    print(f' * An open-source cross-platform traceroute analyser.')
    print(f' *')
    print(f' * Created by {author} on {dateString}.')
