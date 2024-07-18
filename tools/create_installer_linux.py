import os
import platform
import shutil
import subprocess
import sys
import tempfile


def get_architecture():
    arch = platform.processor()
    arch_map = {
        "x86_64": "amd64",
        "i686": "i386",
        "aarch64": "arm64",
    }
    return arch_map.get(arch, arch)


APP_NAME = "CapyArtStudio"
VERSION = "1.0"  # TODO: Get from outside source
ARCHITECTURE = get_architecture()
MAINTAINER = "Ziperix <ziperix@icloud.com>"
DESCRIPTION = "Pixel Art editor"
DEPENDENCIES = "libc6 (>= 2.15)"
SOURCE_APP = f"{os.path.abspath(os.path.dirname(__file__))}/../conan/build/application_build/{APP_NAME}"
DEB_NAME = f"{APP_NAME}.{sys.platform}.{VERSION}.{ARCHITECTURE}.deb"


def create_staging_directory():
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
    subprocess.run(["dpkg-deb", "--build", staging_dir, DEB_NAME], check=True)
    return DEB_NAME


def clean_up(staging_dir):
    shutil.rmtree(staging_dir)


def main():
    if len(sys.argv) != 1:
        print("Usage: python3 create_installer_linux.py")
        exit()
    if sys.platform != "linux":
        print("This script can only be used on Linux system")
        exit()

    staging_dir = create_staging_directory()
    try:
        create_directory_structure(staging_dir)
        create_control_file(staging_dir)
        copy_application(staging_dir)
        deb_file = build_deb_package(staging_dir)
        print(f"DEB package created: {deb_file}")
    finally:
        clean_up(staging_dir)


if __name__ == "__main__":
    main()
