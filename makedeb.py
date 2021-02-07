#!/bin/env python3

# Copyright (C) 2019 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 07/02/2021.
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

import io
import subprocess
import glob
import os
import re
import shutil
import string

# TODO: integrate into deploy.py and add "--deb" as an option and also "--appimage"

def execute(command):
    output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    return(output.returncode, output.stdout.decode('utf-8')+output.stderr.decode('utf-8'))

controlTemplate = ""

with open("dpkg/control.in", 'r') as controlFile:
	controlTemplate = string.Template(controlFile.read())

	controlFile.close()

buildArch = "x86_64"
buildType = "Release"
version = "2021.10.01"

# remove previous dpkg build tree if it exists

if os.path.exists(f'bin/{buildArch}/Deploy/dpkg'):
	shutil.rmtree(f'bin/{buildArch}/Deploy/dpkg')

os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')
os.makedirs(f'bin/{buildArch}/Deploy/dpkg/DEBIAN')
os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/share/icons/hicolor/128x128/apps')
os.makedirs(f'bin/{buildArch}/Deploy/dpkg/etc/ld.so.conf.d')

shutil.copy2(f'bin/{buildArch}/{buildType}/Pingnoo', f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')
shutil.copy2(f'installer/Pingnoo.png', f'bin/{buildArch}/Deploy/dpkg/usr/share/icons/hicolor/128x128/apps')

shutil.copytree(f'bin/{buildArch}/{buildType}/Components', f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo/Components', symlinks=True)

for file in glob.glob(f'bin/{buildArch}/{buildType}/*.so'):
	shutil.copy2(file, f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')

shutil.copy2(f'dpkg/postinst', f'bin/{buildArch}/Deploy/dpkg/DEBIAN')
shutil.copy2(f'dpkg/pingnoo.conf', f'bin/{buildArch}/Deploy/dpkg/etc/ld.so.conf.d')

dependencies = set()
libraries = set()
hashes = list()

for filepath in glob.iglob(f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo/**/*', recursive=True):
	if os.path.isdir(filepath):
		continue

	resultCode, resultOutput = execute(f'md5sum {filepath}')

	if not resultCode:
		hashes.append(resultOutput.rstrip())

	soRegex = re.compile(r"\s*(?P<soname>.*)\s=>")

	resultCode, resultOutput = execute(f'ldd {filepath}')

	for line in io.StringIO(resultOutput):
		if not line:
			continue

		result = soRegex.match(line.rstrip())

		if not result:
			continue

		dependencies.add(result.group("soname"))

dpkgRegex = re.compile(r"(?P<pkg>.*)\:(?P<arch>.*)\:\s(?P<lib>.*)")

dependencies = dependencies-libraries
packages = ""

for dependency in dependencies:
	resultCode, resultOutput = execute(f'dpkg -S {dependency}')

	result = dpkgRegex.match(resultOutput)

	if result:
		if not packages=="":
			packages += ","

		packages += result.group("pkg")

controlFileContent = controlTemplate.substitute(version=version, dependencies=packages)

with open(f'bin/{buildArch}/Deploy/dpkg/DEBIAN/control', 'w') as controlFile:
	controlFile.write(controlFileContent)

	controlFile.close()

with open(f'bin/{buildArch}/Deploy/dpkg/DEBIAN/md5sums', 'w') as md5sumsFile:
	md5sumsFile.writelines(hashes)

	md5sumsFile.close()

packageRoot = f'bin/{buildArch}/Deploy/dpkg/'
outputFile = "deployment/pingnoo.deb"

resultCode, resultOutput = execute(f'dpkg-deb --build {packageRoot} {outputFile}')

