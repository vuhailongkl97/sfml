cmake_minimum_required(VERSION 3.16.3)

project(sfml)

add_subdirectory (solar_system)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 1)

# for packing source to release
include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
