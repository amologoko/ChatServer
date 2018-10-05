import os
from pathlib import Path
from conans import ConanFile, CMake


class ChatServer(ConanFile):
    """ChatServer with GTest"""
    name = "ChatServer"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = "gtest/1.8.1@bincrafters/stable", "jsonformoderncpp/3.2.0@vthiery/stable", "asio/1.12.0@bincrafters/stable"
    generators="cmake"

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        definitions = {"CMAKE_BUILD_TYPE": self.settings.build_type, "BUILD_SHARED_LIBS": False}
        cmake.configure(defs=definitions)
        cmake.build()
        #self.copy("*.dylib*", "./bin", "./lib", folder=True)
        cmake.test()
        
    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", src="lib", keep_path=False)
        self.copy("*.dll", dst="bin", src="bin", keep_path=False)
        self.copy("*.dylib", dst="bin", src="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = ["ChatServer"]
