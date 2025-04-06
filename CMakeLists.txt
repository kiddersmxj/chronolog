cmake_minimum_required(VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)

project(chronolog
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(chronolog src/chronolog.cpp)

find_package(std-k REQUIRED)

target_include_directories(chronolog
    PRIVATE inc
)

install(
    TARGETS chronolog
    DESTINATION /usr/local/bin/
)

target_link_libraries(chronolog
    PRIVATE std-k
)

target_compile_features(chronolog PUBLIC cxx_std_20)

target_sources(chronolog
    PRIVATE src/chronolog.cpp
    PRIVATE src/config.cpp
    PRIVATE src/logger.cpp
    PRIVATE src/timer.cpp
)

# Copyright (c) 2024, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

