import os

from conan.tools.cmake import CMake
from conan.tools.files import load

from conan import ConanFile


class CapyArtStudio(ConanFile):
    name = "CapyArtStudio"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    # TODO: Move building tests seperatetly to here conan test? Don't build both at build()

    def requirements(self):
        self.requires("spdlog/1.14.1")
        self.requires("fmt/10.2.1")
        self.requires("tomlplusplus/3.4.0")
        self.requires("gtest/1.14.0")
        self.requires("rapidjson/1.1.0")
        self.requires("libzippp/7.1-1.10.1")

    def layout(self):
        self.folders.root = "build"
        self.folders.source = "../.."
        self.folders.build = "application_build"

    def build(self):
        path = os.path.join(self.source_folder, "CMakeLists.txt")
        cmake_file = load(self, path)
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
