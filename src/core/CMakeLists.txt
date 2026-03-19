add_library(OptiPlayerCore STATIC
    player.cpp
    decoder.cpp
    syncing.cpp
)

target_include_directories(OptiPlayerCore 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Options pour forcer le mode sans dépendances
option(USE_SYSTEM_FFMPEG "Use system FFmpeg" OFF)
option(USE_SYSTEM_MPV "Use system MPV" OFF)

if(NOT USE_SYSTEM_FFMPEG)
    message(STATUS "🔧 Using FFmpeg stub mode")
    # Créer des cibles factices pour FFmpeg
    add_library(FFMPEG::avcodec INTERFACE IMPORTED)
    add_library(FFMPEG::avformat INTERFACE IMPORTED)
    add_library(FFMPEG::avutil INTERFACE IMPORTED)
    add_library(FFMPEG::swscale INTERFACE IMPORTED)
else()
    # Chercher FFmpeg normalement
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
endif()

if(NOT USE_SYSTEM_MPV)
    message(STATUS "🔧 Using MPV stub mode")
    # Créer une cible factice pour MPV
    add_library(MPV::libmpv INTERFACE IMPORTED)
    
    # Créer un répertoire d'inclusion factice
    set(MPV_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/mpv_stub")
    file(MAKE_DIRECTORY "${MPV_INCLUDE_DIR}")
    
    # Créer un fichier client.h factice
    file(WRITE "${MPV_INCLUDE_DIR}/client.h"
        "#pragma once\n"
        "typedef void mpv_handle;\n"
        "enum mpv_format { MPV_FORMAT_DOUBLE, MPV_FORMAT_INT64, MPV_FORMAT_FLAG, MPV_FORMAT_STRING };\n"
        "static inline mpv_handle* mpv_create() { return nullptr; }\n"
        "static inline int mpv_initialize(mpv_handle*) { return 0; }\n"
        "static inline void mpv_terminate_destroy(mpv_handle*) {}\n"
        "static inline int mpv_set_option_string(mpv_handle*, const char*, const char*) { return 0; }\n"
        "static inline int mpv_set_property(mpv_handle*, const char*, mpv_format, void*) { return 0; }\n"
        "static inline int mpv_set_property_string(mpv_handle*, const char*, const char*) { return 0; }\n"
        "static inline int mpv_get_property(mpv_handle*, const char*, mpv_format, void*) { return 0; }\n"
        "static inline int mpv_command(mpv_handle*, const char**) { return 0; }\n"
    )
    
    target_include_directories(OptiPlayerCore PRIVATE "${MPV_INCLUDE_DIR}")
else()
    # Chercher MPV normalement
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MPV REQUIRED mpv)
endif()

# fmt, spdlog, nlohmann_json (ceux-ci fonctionnent)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Lier les bibliothèques
target_link_libraries(OptiPlayerCore
    PRIVATE
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

set_target_properties(OptiPlayerCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
