import os
import shutil
import subprocess
import sys
import tempfile

# Configuration variables
APP_NAME = "CapyArtStudio"
VERSION = "1.0"
ARCHITECTURE = "amd64"  # TODO platform.machine() ?
MAINTAINER = "Ziperix <ziperix@icloud.com>"
DESCRIPTION = "Pixel Art editor"
DEPENDENCIES = "libc6 (>= 2.15)"  # TODO qt?
SOURCE_APP = f"{os.path.abspath(os.path.dirname(__file__))}/../conan/build/application_build/CapyArtStudio"
STAGING_DIR = "linux_installer_temporary_staging_folder"  # Specify a custom path or leave empty for temp directory


def create_staging_directory(staging_dir):
    if staging_dir:
        os.makedirs(staging_dir, exist_ok=True)
        return staging_dir
    else:
        return tempfile.mkdtemp()


def create_directory_structure(staging_dir):
    os.makedirs(os.path.join(staging_dir, "DEBIAN"), exist_ok=True)
    os.makedirs(os.path.join(staging_dir, "usr/local/bin"), exist_ok=True)


def create_control_file(staging_dir):
    control_content = f"""Package: {APP_NAME}
Version: {VERSION}
Section: base
Priority: optional
Architecture: {ARCHITECTURE}
Depends: {DEPENDENCIES}
Maintainer: {MAINTAINER}
Description: {DESCRIPTION}
"""
    with open(os.path.join(staging_dir, "DEBIAN/control"), 'w') as control_file:
        control_file.write(control_content)


def copy_application(staging_dir):
    destination = os.path.join(staging_dir, "usr/local/bin", APP_NAME)
    shutil.copy(SOURCE_APP, destination)


def build_deb_package(staging_dir):
    subprocess.run(["dpkg-deb", "--build", staging_dir], check=True)
    deb_file = f"{APP_NAME}_{VERSION}_{ARCHITECTURE}.deb"
    shutil.move(f"{staging_dir}.deb", deb_file)
    return deb_file


def clean_up(staging_dir, use_temp_dir):
    if use_temp_dir:
        shutil.rmtree(staging_dir)


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 create_installer_linux.py [DESTINATION]")
        exit()
    if sys.platform != "linux":
        print("This script can only be used on Linux system")
        exit()

    destination = sys.argv[1]

    use_temp_dir = not STAGING_DIR
    staging_dir = create_staging_directory(STAGING_DIR)
    try:
        create_directory_structure(staging_dir)
        create_control_file(staging_dir)
        copy_application(staging_dir)
        deb_file = build_deb_package(staging_dir)
        print(f"DEB package created: {deb_file}")
    finally:
        clean_up(staging_dir, use_temp_dir)


if __name__ == "__main__":
    main()
