# CMakeList.txt : CMake project for Encryptor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Encryptor")

# Add source to this project's executable.
set(SOURCE_FILES 
    "src/encryptor.cpp" "src/encryptor.h" 
    "resources/resource.rc" 
    "src/encryption/encrypt.cpp" "src/encryption/encrypt.h" 
    "src/encryption/decrypt.cpp" "src/encryption/decrypt.h" 
    "src/tools/eprint.cpp" "src/tools/eprint.h" 
    "src/tools/edir.cpp" "src/tools/edir.h"
    "src/shell/shell.cpp" "src/shell/shell.h" 
)
add_executable(Encryptor ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Encryptor PROPERTY CXX_STANDARD 20)
endif()

# Explicitly set output directories to avoid confusion
set_target_properties(Encryptor PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Set the startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Encryptor)

# TODO: Add tests and install targets if needed.
