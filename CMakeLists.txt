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

# Support for manually downloaded FFmpeg
if(DEFINED ENV{FFMPEG_DIR})
    set(FFMPEG_ROOT $ENV{FFMPEG_DIR})
    message(STATUS "Using FFmpeg from: ${FFMPEG_ROOT}")
endif()

# Support for manually downloaded libmpv
if(DEFINED ENV{MPV_DIR})
    set(MPV_ROOT $ENV{MPV_DIR})
    message(STATUS "Using MPV from: ${MPV_ROOT}")
    
    # Trouver libmpv
    find_path(MPV_INCLUDE_DIR
        NAMES mpv/client.h
        PATHS ${MPV_ROOT}/include
        NO_DEFAULT_PATH
    )
    
    find_library(MPV_LIBRARY
        NAMES mpv mpv-1
        PATHS ${MPV_ROOT}/lib ${MPV_ROOT}
        NO_DEFAULT_PATH
    )
    
    if(MPV_INCLUDE_DIR AND MPV_LIBRARY)
        add_library(MPV::libmpv UNKNOWN IMPORTED)
        set_target_properties(MPV::libmpv PROPERTIES
            IMPORTED_LOCATION "${MPV_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${MPV_INCLUDE_DIR}"
        )
        message(STATUS "Found MPV: ${MPV_LIBRARY}")
    endif()
endif()
