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

# Afficher les variables d'environnement pour debug
message(STATUS "FFMPEG_DIR = $ENV{FFMPEG_DIR}")
message(STATUS "MPV_DIR = $ENV{MPV_DIR}")

if(WIN32)
    # FFmpeg
    if(DEFINED ENV{FFMPEG_DIR} AND EXISTS "$ENV{FFMPEG_DIR}")
        set(FFMPEG_ROOT "$ENV{FFMPEG_DIR}")
        message(STATUS "Looking for FFmpeg in: ${FFMPEG_ROOT}")
        
        # Chercher les include
        find_path(FFMPEG_AVCODEC_INCLUDE_DIR
            NAMES libavcodec/avcodec.h
            PATHS ${FFMPEG_ROOT}/include
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        # Chercher les libs
        find_library(FFMPEG_AVCODEC_LIBRARY
            NAMES avcodec
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        find_library(FFMPEG_AVFORMAT_LIBRARY
            NAMES avformat
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        find_library(FFMPEG_AVUTIL_LIBRARY
            NAMES avutil
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        find_library(FFMPEG_SWSCALE_LIBRARY
            NAMES swscale
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        if(FFMPEG_AVCODEC_INCLUDE_DIR AND FFMPEG_AVCODEC_LIBRARY)
            message(STATUS "✅ Found FFmpeg: ${FFMPEG_AVCODEC_LIBRARY}")
            
            add_library(FFMPEG::avcodec UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::avcodec PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
            
            add_library(FFMPEG::avformat UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::avformat PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_AVFORMAT_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
            
            add_library(FFMPEG::avutil UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::avutil PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_AVUTIL_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
            
            add_library(FFMPEG::swscale UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::swscale PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_SWSCALE_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
        else()
            message(FATAL_ERROR "❌ FFmpeg not found in ${FFMPEG_ROOT}")
        endif()
    else()
        message(FATAL_ERROR "❌ FFMPEG_DIR environment variable not set or directory does not exist")
    endif()
    
    # MPV
    if(DEFINED ENV{MPV_DIR} AND EXISTS "$ENV{MPV_DIR}")
        set(MPV_ROOT "$ENV{MPV_DIR}")
        message(STATUS "Looking for MPV in: ${MPV_ROOT}")
        
        find_path(MPV_INCLUDE_DIR
            NAMES mpv/client.h
            PATHS ${MPV_ROOT}/include
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        find_library(MPV_LIBRARY
            NAMES mpv mpv-1
            PATHS ${MPV_ROOT}/lib ${MPV_ROOT}
            NO_DEFAULT_PATH
            REQUIRED
        )
        
        if(MPV_INCLUDE_DIR AND MPV_LIBRARY)
            message(STATUS "✅ Found MPV: ${MPV_LIBRARY}")
            
            add_library(MPV::libmpv UNKNOWN IMPORTED)
            set_target_properties(MPV::libmpv PROPERTIES
                IMPORTED_LOCATION "${MPV_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${MPV_INCLUDE_DIR}"
            )
        else()
            message(FATAL_ERROR "❌ MPV not found in ${MPV_ROOT}")
        endif()
    else()
        message(FATAL_ERROR "❌ MPV_DIR environment variable not set or directory does not exist")
    endif()
    
    # Lier les bibliothèques
    target_link_libraries(OptiPlayerCore
        PRIVATE
            FFMPEG::avcodec
            FFMPEG::avformat
            FFMPEG::avutil
            FFMPEG::swscale
            MPV::libmpv
            fmt::fmt
            spdlog::spdlog
            nlohmann_json::nlohmann_json
    )
    
else()
    # Pour Linux/Mac
    target_link_libraries(OptiPlayerCore
        PRIVATE
            ${FFMPEG_LIBRARIES}
            ${MPV_LIBRARIES}
            fmt::fmt
            spdlog::spdlog
            nlohmann_json::nlohmann_json
    )
    
    target_include_directories(OptiPlayerCore
        PRIVATE
            ${FFMPEG_INCLUDE_DIRS}
            ${MPV_INCLUDE_DIRS}
    )
endif()

set_target_properties(OptiPlayerCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
