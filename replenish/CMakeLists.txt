# CMakeList.txt: файл проекта CMake верхнего уровн€; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

# ¬ключение гор€чей перезагрузки дл€ компил€торов MSVC, если поддерживаетс€.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
project ("idena_replenish")
set(CMAKE_CXX_STANDARD 17)
set(DEFAULT_BUILD_TYPE "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(DEFAULT_BUILD_TYPE "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_executable(idena_replenish idena_replenish.cpp)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.10.5) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases

FetchContent_Declare(json GIT_REPOSITORY https://github.com/open-source-parsers/json
                         GIT_TAG v1.0.0)

FetchContent_Declare(cli11 URL https://github.com/CLIUtils/CLI11/releases/download/v2.4.2/CLI11.hpp DOWNLOAD_NO_EXTRACT true)
FetchContent_MakeAvailable(cli11)
FetchContent_GetProperties(cli11 SOURCE_DIR cli11srcDirVar)
FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(json)
FetchContent_GetProperties(json SOURCE_DIR jsonsrcDirVar)
target_link_libraries(idena_replenish PRIVATE cpr::cpr)
target_include_directories(idena_replenish PUBLIC "${jsonsrcDirVar}" "${cli11srcDirVar}")
# ¬ключите подпроекты.
#add_subdirectory ("replenish")
