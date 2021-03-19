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
import argparse

from .msg_printer import msg_printer

def execute(command):
    output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    return(output.returncode, output.stdout.decode('utf-8')+output.stderr.decode('utf-8'))

def debCreate(buildArch, buildType, version, outputFile):
	controlTemplate = ""

	with open("dpkg/control.in", 'r') as controlFile:
		controlTemplate = string.Template(controlFile.read())

		controlFile.close()

	# remove previous dpkg build tree if it exists

	if os.path.exists(f'bin/{buildArch}/Deploy/dpkg'):
		shutil.rmtree(f'bin/{buildArch}/Deploy/dpkg')

	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')
	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/DEBIAN')
	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/share/icons/hicolor/512x512/apps')
	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/share/applications')
	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/usr/share/doc/pingnoo')
	os.makedirs(f'bin/{buildArch}/Deploy/dpkg/etc/ld.so.conf.d')

	# copy data + binaries into the deb tree

	shutil.copy2(f'bin/{buildArch}/{buildType}/Pingnoo', f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')
	shutil.copy2(f'src/app/images/appicon-512x512-.png', f'bin/{buildArch}/Deploy/dpkg/usr/share/icons/hicolor/512x512/apps/pingnoo.png')

	shutil.copytree(f'bin/{buildArch}/{buildType}/Components', f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo/Components', symlinks=True)

	for file in glob.glob(f'bin/{buildArch}/{buildType}/*.so'):
		shutil.copy2(file, f'bin/{buildArch}/Deploy/dpkg/usr/local/bin/pingnoo')

	shutil.copy2(f'dpkg/postinst', f'bin/{buildArch}/Deploy/dpkg/DEBIAN')
	shutil.copy2(f'dpkg/pingnoo.conf', f'bin/{buildArch}/Deploy/dpkg/etc/ld.so.conf.d')
	shutil.copy2(f'dpkg/copyright', f'bin/{buildArch}/Deploy/dpkg/usr/share/doc/pingnoo')
	shutil.copy2(f'dpkg/Pingnoo.desktop', f'bin/{buildArch}/Deploy/dpkg/usr/share/applications')

	dependencies = set()
	libraries = set()
	hashes = list()

	# create list of all shared libraries that the application uses (and at the same time create hashes)

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

	# using the shared library list, find the package that provides the shared library and add to
	# the list of dependencies

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

	# use control.in template to create the deb control file

	controlFileContent = controlTemplate.substitute(version=version, dependencies=packages)

	with open(f'bin/{buildArch}/Deploy/dpkg/DEBIAN/control', 'w') as controlFile:
		controlFile.write(controlFileContent)

		controlFile.close()

	# add the hashes to the deb hash file

	with open(f'bin/{buildArch}/Deploy/dpkg/DEBIAN/md5sums', 'w') as md5sumsFile:
		md5sumsFile.writelines(hashes)

		md5sumsFile.close()

	packageRoot = f'bin/{buildArch}/Deploy/dpkg/'

	# remove any previous deployment artifacts

	with msg_printer("Removing existing deployment artifacts"):
		execute("rm -rf deployment", "Could not remove deployment folder")
	
	# create the deb file

	resultCode, resultOutput = execute(f'dpkg-deb --build {packageRoot} \"{outputFile}\"')

	if resultCode==0:
		return(0)

	return(1)

def main():
	parser = argparse.ArgumentParser(description='dpkg build script')

	parser.add_argument('--arch',
						choices=['x86_64'],
						type=str,
						default='x86_64',
						nargs='?',
						help='architecture type to deploy')

	parser.add_argument('--type',
						choices=['Release', 'Debug'],
						type=str,
						default='Release',
						nargs='?',
						help='architecture type to deploy')

	parser.add_argument('--output',
						type=str,
						default='./deployment/pingnoo.deb',
						nargs='?',
						help='the output deb file')

	parser.add_argument('--version', type=str, nargs='?', help='version')

	args = parser.parse_args()

	debCreate(args.arch, args.type, args.version, args.output)

if __name__ == "__main__":
	main()

