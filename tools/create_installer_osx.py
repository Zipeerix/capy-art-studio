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
    if len(sys.argv) != 2:
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
