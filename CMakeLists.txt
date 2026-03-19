cmake_minimum_required(VERSION 3.20)
project(OptiPlayer VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)

# Options
option(ENABLE_INSTALLER "Generate installer" OFF)

# Chemins des dépendances externes
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(FFMPEG_DIR "${EXTERNAL_DIR}/ffmpeg")
set(MPV_DIR "${EXTERNAL_DIR}/mpv")

message(STATUS "════════════════════════════════════════════")
message(STATUS "🔧 CONFIGURATION OPTIPLAYER")
message(STATUS "════════════════════════════════════════════")
message(STATUS "FFmpeg: ${FFMPEG_DIR}")
message(STATUS "MPV: ${MPV_DIR}")

# ====================================================
# FFMPEG
# ====================================================
message(STATUS "🔍 Recherche de FFmpeg...")

find_path(FFMPEG_INCLUDE_DIR
    NAMES libavcodec/avcodec.h
    PATHS ${FFMPEG_DIR}/include
    REQUIRED
    NO_DEFAULT_PATH
)

find_library(FFMPEG_AVCODEC_LIBRARY
    NAMES avcodec
    PATHS ${FFMPEG_DIR}/lib ${FFMPEG_DIR}/lib/x64
    REQUIRED
    NO_DEFAULT_PATH
)

find_library(FFMPEG_AVFORMAT_LIBRARY
    NAMES avformat
    PATHS ${FFMPEG_DIR}/lib ${FFMPEG_DIR}/lib/x64
    REQUIRED
    NO_DEFAULT_PATH
)

find_library(FFMPEG_AVUTIL_LIBRARY
    NAMES avutil
    PATHS ${FFMPEG_DIR}/lib ${FFMPEG_DIR}/lib/x64
    REQUIRED
    NO_DEFAULT_PATH
)

find_library(FFMPEG_SWSCALE_LIBRARY
    NAMES swscale
    PATHS ${FFMPEG_DIR}/lib ${FFMPEG_DIR}/lib/x64
    REQUIRED
    NO_DEFAULT_PATH
)

if(FFMPEG_INCLUDE_DIR AND FFMPEG_AVCODEC_LIBRARY)
    message(STATUS "✅ FFmpeg trouvé:")
    message(STATUS "   - Include: ${FFMPEG_INCLUDE_DIR}")
    message(STATUS "   - avcodec: ${FFMPEG_AVCODEC_LIBRARY}")
    
    add_library(FFmpeg::avcodec UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avcodec PROPERTIES
        IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
    )
    
    add_library(FFmpeg::avformat UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avformat PROPERTIES
        IMPORTED_LOCATION "${FFMPEG_AVFORMAT_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
    )
    
    add_library(FFmpeg::avutil UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avutil PROPERTIES
        IMPORTED_LOCATION "${FFMPEG_AVUTIL_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
    )
    
    add_library(FFmpeg::swscale UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::swscale PROPERTIES
        IMPORTED_LOCATION "${FFMPEG_SWSCALE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
    )
else()
    message(FATAL_ERROR "❌ FFmpeg non trouvé dans ${FFMPEG_DIR}")
endif()

# ====================================================
# MPV
# ====================================================
message(STATUS "🔍 Recherche de MPV...")

find_path(MPV_INCLUDE_DIR
    NAMES mpv/client.h
    PATHS ${MPV_DIR}/include
    REQUIRED
    NO_DEFAULT_PATH
)

find_library(MPV_LIBRARY
    NAMES mpv mpv-1
    PATHS ${MPV_DIR}/lib ${MPV_DIR}
    REQUIRED
    NO_DEFAULT_PATH
)

if(MPV_INCLUDE_DIR AND MPV_LIBRARY)
    message(STATUS "✅ MPV trouvé:")
    message(STATUS "   - Include: ${MPV_INCLUDE_DIR}")
    message(STATUS "   - Library: ${MPV_LIBRARY}")
    
    add_library(MPV::libmpv UNKNOWN IMPORTED)
    set_target_properties(MPV::libmpv PROPERTIES
        IMPORTED_LOCATION "${MPV_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${MPV_INCLUDE_DIR}"
    )
else()
    message(FATAL_ERROR "❌ MPV non trouvé dans ${MPV_DIR}")
endif()

# ====================================================
# vcpkg pour les petites dépendances
# ====================================================
message(STATUS "🔍 Recherche des dépendances vcpkg...")

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

message(STATUS "✅ fmt: trouvé")
message(STATUS "✅ spdlog: trouvé")
message(STATUS "✅ nlohmann_json: trouvé")

# ====================================================
# Sous-projets
# ====================================================
add_subdirectory(src/core)
add_subdirectory(src/ui)

message(STATUS "════════════════════════════════════════════")
message(STATUS "🎉 CONFIGURATION TERMINÉE AVEC SUCCÈS")
message(STATUS "════════════════════════════════════════════")
