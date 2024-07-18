"""
Copyright (C) 2024 CapyArt Studio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <https://www.gnu.org/licenses/>.
"""

import os
import platform
import shutil
import subprocess
import sys

APP_NAME = "CapyArtStudio"
APP_BUNDLE = f"{APP_NAME}.app"
VERSION = "1.0"  # TODO: Get from outside source
STAGING_DIR = "macos_installer_temporary_staging_folder"
ARCHITECTURE = platform.machine()  # TODO: Match with linux installer
DMG_NAME = f"{APP_NAME}.{sys.platform}.{VERSION}.{ARCHITECTURE}.dmg"
VOLUME_NAME = APP_NAME
SOURCE_APP = f"{os.path.abspath(os.path.dirname(__file__))}/../conan/build/application_build/{APP_BUNDLE}"


def create_staging_directory(staging_dir):
    if not os.path.exists(staging_dir):
        os.makedirs(staging_dir)


def copy_app_to_staging(source_app, staging_dir):
    destination = os.path.join(staging_dir, APP_BUNDLE)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    shutil.copytree(source_app, destination)


def create_dmg(dmg_name, volume_name, staging_dir):
    subprocess.run([
        "hdiutil", "create", "-volname", volume_name, "-srcfolder", staging_dir,
        "-ov", "-format", "UDZO", dmg_name
    ], check=True)
    print(f"Installer DMG file created: {dmg_name}")


def clean_up(staging_dir):
    if os.path.exists(staging_dir):
        shutil.rmtree(staging_dir)


def main():
    if len(sys.argv) != 1:
        print("Usage: python3 create_installer_osx.py")
        exit()
    if sys.platform != "darwin":
        print("This script can only be used on OSX system")
        exit()

    try:
        create_staging_directory(STAGING_DIR)
        copy_app_to_staging(SOURCE_APP, STAGING_DIR)
        create_dmg(DMG_NAME, VOLUME_NAME, STAGING_DIR)
    finally:
        clean_up(STAGING_DIR)


if __name__ == "__main__":
    main()
