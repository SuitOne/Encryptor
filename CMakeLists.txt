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
# Add the resource file for this icon
if(WIN32)
	set(SOURCE_FILES "src/encryptor.cpp" "src/encryptor.h" "resources/resource.rc" "src/encrypt.cpp" "src/encrypt.h" "src/decrypt.cpp" "src/decrypt.h" "src/eprint.cpp" "src/eprint.h")
	add_executable(Encryptor ${SOURCE_FILES})
else()
	add_executable(Encryptor "src/Encryptor.cpp" "src/Encryptor.h" "src/Encrypt.cpp" "src/Encrypt.h" "src/Decrypt.cpp" "src/Decrypt.h" "src/eprint.cpp" "src/eprint.h")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Encryptor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
