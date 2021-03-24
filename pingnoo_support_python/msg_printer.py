#!/bin/env python3

# Copyright (C) 2021 Aaron D. Marasco
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

""" A context handler to print things to the console nicely """
import platform
import sys
from contextlib import contextmanager

try:
    from colorama import Fore, Style, init

    init(autoreset=True)

except ModuleNotFoundError:
    # Make some blank wrappers
    class Style:
        BRIGHT = ''
        RESET = ''


    class Fore:
        GREEN = ''
        RED = ''
        CYAN = ''
        RESET = ''


class MsgPrinterException(Exception):
    pass


GOOD_SYMBOL = 'Y' if platform.system() == "Windows" else '✓'
BAD_SYMBOL = 'N' if platform.system() == "Windows" else '✘'


@contextmanager
def msg_printer(start_msg=None):
    """ Context to use to print messages. Will exit if receives MsgPrinterException or re-raise any other """
    try:
        if start_msg:
            sys.stdout.write(Style.BRIGHT + f'> {start_msg} ')
            sys.stdout.flush()
            yield None
            sys.stdout.write(Style.BRIGHT + '[' + Fore.GREEN + GOOD_SYMBOL + Fore.RESET + ']\r\n')
            sys.stdout.flush()
        else:
            yield None
    except Exception as err:
        if start_msg:
            sys.stdout.write(Style.BRIGHT + '[' + Fore.RED + BAD_SYMBOL + Fore.RESET + ']\r\n')
        if isinstance(err, MsgPrinterException):
            print('\r\n' + Fore.RED + 'ERROR: ' + Fore.RESET + str(err), flush=True)
            sys.exit(1)
        raise
