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
import subprocess
import sys

APP_NAME = "CapyArtStudio"
VERSION = "1.0"
ARCHITECTURE = platform.machine()  # TODO: Match with linux installer and macos
SOURCE_APP = fr"{os.path.abspath(os.path.dirname(__file__))}\..\conan\build\application_build\Release\{APP_NAME}.exe"
OUTPUT_DIR = r"."
INSTALLER_NAME = f"{APP_NAME}.{sys.platform}.{VERSION}.{ARCHITECTURE}"


def create_inno_setup_script(app_name, version, source_app, output_dir, installer_name):
    script_content = f"""
[Setup]
AppId={{YOUR-GUID-HERE}}
AppName={app_name}
AppVersion={version}
DefaultDirName={{pf}}\\{app_name}
DefaultGroupName={app_name}
OutputDir={output_dir}
OutputBaseFilename={installer_name}
Compression=lzma
SolidCompression=yes

[Files]
Source: "{source_app}"; DestDir: "{{{{app}}}}"; Flags: ignoreversion

[Icons]
Name: "{{{{group}}}}\\{app_name}"; Filename: "{{{{app}}}}\\{os.path.basename(source_app)}"
Name: "{{{{group}}}}\\Uninstall {app_name}"; Filename: "{{{{uninstallexe}}}}"

[Run]
Filename: "{{{{app}}}}\\{os.path.basename(source_app)}"; Description: "Launch {app_name}"; Flags: nowait postinstall skipifsilent
"""
    script_path = os.path.join(output_dir, "setup.iss")
    with open(script_path, 'w') as script_file:
        script_file.write(script_content)
    return script_path


def compile_inno_setup_script(script_path):
    subprocess.run(["ISCC", script_path], check=True)


def main():
    if len(sys.argv) != 1:
        print("Usage: python3 create_installer_windows.py")
        exit()
    if sys.platform != "win32" and sys.platform != "win64":
        print("This script can only be used on Windows system")
        exit()

    script_path = create_inno_setup_script(APP_NAME, VERSION, SOURCE_APP, OUTPUT_DIR, INSTALLER_NAME)
    compile_inno_setup_script(script_path)
    print(f"Installer created: {os.path.join(OUTPUT_DIR, INSTALLER_NAME)}.exe")


if __name__ == "__main__":
    main()
