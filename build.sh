#!/bin/bash

pip install conan
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add vthiery https://api.bintray.com/conan/vthiery/conan-packages
conan install -s build_type=Debug .
conan build .
