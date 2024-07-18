import os
import shutil
import subprocess
import sys

# Configuration variables
APP_NAME = "CapyArtStudio"
VERSION = "1.0"
ARCHITECTURE = "arm64"
MAINTAINER = "Ziperix <ziperix@icloud.com>"
DESCRIPTION = "Pixel Art editor."
DEPENDENCIES = "libc6 (>= 2.15)"
SOURCE_APP = f"{os.path.abspath(os.path.dirname(__file__))}/../conan/build/application_build/{APP_NAME}"


def create_directory_structure():
    os.makedirs(os.path.join(SOURCE_APP, "DEBIAN"), exist_ok=True)
    os.makedirs(os.path.join(SOURCE_APP, "usr/local/bin"), exist_ok=True)


def create_control_file():
    control_content = f"""Package: {APP_NAME}
Version: {VERSION}
Section: base
Priority: optional
Architecture: {ARCHITECTURE}
Depends: {DEPENDENCIES}
Maintainer: {MAINTAINER}
Description: {DESCRIPTION}
"""
    with open(os.path.join(SOURCE_APP, "DEBIAN/control"), 'w') as control_file:
        control_file.write(control_content)


def copy_application():
    shutil.copy(SOURCE_APP, os.path.join(SOURCE_APP, "usr/local/bin", APP_NAME))


def build_deb_package():
    subprocess.run(["dpkg-deb", "--build", SOURCE_APP], check=True)
    shutil.move(f"{SOURCE_APP}.deb", f"{APP_NAME}_{VERSION}_{ARCHITECTURE}.deb")


def clean_up():
    shutil.rmtree(SOURCE_APP)


def main():
    if len(sys.argv) != 1:
        print("Usage: python3 create_installer_linux.py")
        exit()
    if sys.platform != "linux":
        print("This script can only be used on Linux system")
        exit()

    create_directory_structure()
    create_control_file()
    copy_application()
    build_deb_package()
    clean_up()
    print(f"DEB package created: {APP_NAME}_{VERSION}_{ARCHITECTURE}.deb")


if __name__ == "__main__":
    main()
