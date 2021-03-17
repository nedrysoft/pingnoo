#!/usr/bin/env python3

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

""" Common python function used by support python """

import os
import platform
import shutil
import subprocess
import sys

from .msg_printer import MsgPrinterException

# What we want to export:
__all__ = ['Style', 'Fore']
try:
    from colorama import Fore, Style, init
    init(autoreset=True)

except ModuleNotFoundError:
    class Style:
        """ Blank fake version of colorama """
        BRIGHT = ''
        RESET = ''


    class Fore:
        """ Blank fake version of colorama """
        GREEN = ''
        RED = ''
        CYAN = ''
        RESET = ''


__all__.append('ExecuteException')
class ExecuteException(MsgPrinterException):
    """ Custom class to separate msgprinter vs. this code """


__all__.append('timedelta')
def timedelta(seconds):
    """ Pretty print a time interval """
    seconds = abs(int(seconds))

    days, seconds = divmod(seconds, 86400)
    hours, seconds = divmod(seconds, 3600)
    minutes, seconds = divmod(seconds, 60)

    if days:
        return f'{days}d{hours}h{minutes}m{seconds}s'
    if hours:
        return f'{hours}h{minutes}m{seconds}s'
    if minutes:
        return f'{minutes}m{seconds}s'
    return f'{seconds}s'


__all__.append('write_msg')
def write_msg(msg):
    """ Neutral message printer """
    sys.stdout.write(Style.BRIGHT + msg + Fore.RESET + '\r\n')


__all__.append('bad_msg')
def bad_msg(msg):
    """ Bad message printer """
    sys.stdout.write(Style.BRIGHT + Fore.RED + msg + Fore.RESET + '\r\n')


__all__.append('parent')
def parent(path):
    """ Parent path """
    return os.path.normpath(os.path.join(path, os.pardir))


__all__.append('rm_file')
def rm_file(path):
    """ Will remove file if it exists; no warnings if not """
    if os.path.isfile(path):
        os.remove(path)


__all__.append('rm_path')
def rm_path(path):
    """ Will remove path if it exists; no warnings if not """
    if os.path.exists(path):
        shutil.rmtree(path)


__all__.append('execute')
def execute(command, fail_msg=None, out_log=None):
    """ Execute a command in subprocess and throw exceptions if given fail_msg """
    output = subprocess.run(command,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            check=False)
    output_text = output.stdout.decode('utf-8') + output.stderr.decode('utf-8')
    if out_log:
        with open(out_log, 'w') as out_log_file:
            out_log_file.write(output_text)
    if fail_msg and output.returncode:  # Non-zero is error  FIXME: Is this true on Windows?
        raise ExecuteException(fail_msg + "\r\n\r\n" + output_text)
    if fail_msg is None:  # Return backwards-compat results of return code + text
        return output.returncode, output_text
    return output_text


__all__.append('which')
def which(appname):
    """ Find executable in path """
    command = 'where'
    if platform.system() != "Windows":
        command = 'which'  # TODO: 'type' is POSIX; 'which' is not
        execute("which which", fail_msg="Could not find 'which' command!")
    ret, output = execute(f'{command} {appname}')
    if ret:
        return None
    return output.split()[0]


__all__.append('run')
def run(command):
    """ Simplistic os.popen wrapper """
    stream = os.popen(command)
    return stream.read()
