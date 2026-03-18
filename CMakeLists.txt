cmake_minimum_required(VERSION 3.20)
project(OptiPlayer VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OptiPlayer-Release)

option(ENABLE_INSTALLER "Generate installer" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
pkg_check_modules(MPV REQUIRED mpv)

add_subdirectory(src/core)
add_subdirectory(src/ui)

install(TARGETS OptiPlayerCore OptiPlayerUI RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/optiplayer/assets)
install(FILES README.md LICENSE DESTINATION share/optiplayer)

if(ENABLE_INSTALLER)
    set(CPACK_GENERATOR "NSIS64;ZIP")
    set(CPACK_PACKAGE_NAME "OptiPlayer")
    set(CPACK_PACKAGE_VENDOR "OKITAKOY Inc.")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    include(CPack)
endif()
