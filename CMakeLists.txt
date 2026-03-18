cmake_minimum_required(VERSION 3.20)
project(OptiPlayer VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OptiPlayer-Release)

option(ENABLE_INSTALLER "Generate installer" OFF)

# Pas de pkg-config sur Windows, on utilise find_library directement
if(WIN32)
    message(STATUS "Building for Windows - using direct library search")
    
    # FFmpeg
    if(DEFINED ENV{FFMPEG_DIR})
        set(FFMPEG_ROOT $ENV{FFMPEG_DIR})
        message(STATUS "Using FFmpeg from: ${FFMPEG_ROOT}")
        
        find_path(FFMPEG_AVCODEC_INCLUDE_DIR
            NAMES libavcodec/avcodec.h
            PATHS ${FFMPEG_ROOT}/include
            NO_DEFAULT_PATH
        )
        
        find_library(FFMPEG_AVCODEC_LIBRARY
            NAMES avcodec
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64
            NO_DEFAULT_PATH
        )
        
        if(FFMPEG_AVCODEC_INCLUDE_DIR AND FFMPEG_AVCODEC_LIBRARY)
            add_library(FFMPEG::avcodec UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::avcodec PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
            message(STATUS "Found FFmpeg avcodec: ${FFMPEG_AVCODEC_LIBRARY}")
        endif()
    endif()
    
    # libmpv
    if(DEFINED ENV{MPV_DIR})
        set(MPV_ROOT $ENV{MPV_DIR})
        message(STATUS "Using MPV from: ${MPV_ROOT}")
        
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
    
else()
    # Pour Linux/Mac - utiliser pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
    pkg_check_modules(MPV REQUIRED mpv)
endif()

# vcpkg pour les autres dépendances
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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

# Ajouter les fichiers sources manquants
target_sources(OptiPlayerCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/syncing.cpp
)

# Lier les bibliothèques Windows
target_link_libraries(OptiPlayerUI PRIVATE
    comctl32
    gdi32
    user32
    kernel32
)

# Inclure les répertoires
target_include_directories(OptiPlayerCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(OptiPlayerUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
