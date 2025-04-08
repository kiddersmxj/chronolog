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

# --- Install chronolog.conf and copy to $HOME/.chronolog/chronolog.conf if it doesn't exist ----------------

install(CODE "
    # Determine the original user
    if(DEFINED ENV{SUDO_USER} AND NOT \"\$ENV{SUDO_USER}\" STREQUAL \"root\")
        set(ORIGINAL_USER \"\$ENV{SUDO_USER}\")
    else()
        set(ORIGINAL_USER \"\$ENV{USER}\")
    endif()

    # Get the home directory of the original user
    execute_process(
        COMMAND sh -c \"getent passwd \${ORIGINAL_USER} | cut -d: -f6\"
        OUTPUT_VARIABLE USER_HOME
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT USER_HOME)
        message(FATAL_ERROR \"Failed to determine home directory for user '\${ORIGINAL_USER}'\")
    endif()

    set(CONFIG_FILE \"\${USER_HOME}/.chronolog/chronolog.conf\")

    if(NOT EXISTS \"\${CONFIG_FILE}\")
        message(\"Installing configuration file to \${CONFIG_FILE}\")
        file(COPY \"${CMAKE_SOURCE_DIR}/chronolog.conf\" DESTINATION \"\${USER_HOME}\" FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
        file(RENAME \"\${USER_HOME}/chronolog.conf\" \"\${CONFIG_FILE}\")
    else()
        message(\"Configuration file already exists at \${CONFIG_FILE}\")
    endif()
")

# Copyright (c) 2024, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

