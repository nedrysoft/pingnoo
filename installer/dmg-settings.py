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

# dmgbuild -s dmg.py -D app=/bin/Debug/Pingnoo.app "Pingnoo.app" Pingnoo.dmg

from __future__ import unicode_literals

format = 'UDBZ'

compression_level = 9

size = None

files = [ str(defines.get('application_binary')) ]

symlinks = { 'Applications': '/Applications' }

icon_locations = {
    'Pingnoo.app': (165, 365),
    'Applications': (520, 365)
}

background = defines.get('background_image')

show_status_bar = False
show_tab_view = False
show_toolbar = False
show_pathbar = False
show_sidebar = False
sidebar_width = 0

window_rect = ((0, 0), (912, 541))

default_view = 'icon-view'

show_icon_preview = False

include_icon_view_settings = 'auto'
include_list_view_settings = 'auto'

arrange_by = None
grid_offset = (0, 0)
grid_spacing = 10
label_pos = 'bottom'

list_icon_size = 32
list_text_size = 12
list_scroll_position = (0, 0)
list_sort_by = 'name'
list_use_relative_dates = True
list_calculate_all_sizes = False,
list_columns = ('name', 'date-modified', 'size', 'kind', 'date-added')

text_size = 12
icon_size = 128

list_column_widths = {
    'name': 300,
    'date-modified': 181,
    'date-created': 181,
    'date-added': 181,
    'date-last-opened': 181,
    'size': 97,
    'kind': 115,
    'label': 100,
    'version': 75,
    'comments': 300,
}

list_column_sort_directions = {
    'name': 'ascending',
    'date-modified': 'descending',
    'date-created': 'descending',
    'date-added': 'descending',
    'date-last-opened': 'descending',
    'size': 'descending',
    'kind': 'ascending',
    'label': 'ascending',
    'version': 'ascending',
    'comments': 'ascending',
}

