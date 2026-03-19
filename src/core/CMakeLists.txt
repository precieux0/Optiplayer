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
    # FFmpeg - Chercher dans les bons chemins
    if(DEFINED ENV{FFMPEG_DIR} AND EXISTS "$ENV{FFMPEG_DIR}")
        set(FFMPEG_ROOT "$ENV{FFMPEG_DIR}")
        message(STATUS "Looking for FFmpeg in: ${FFMPEG_ROOT}")
        
        # Afficher le contenu du dossier pour debug
        file(GLOB FFMPEG_CONTENTS "${FFMPEG_ROOT}/*")
        message(STATUS "FFmpeg directory contents: ${FFMPEG_CONTENTS}")
        
        # Chercher les include - ESSAYER DIFFÉRENTS CHEMINS
        find_path(FFMPEG_AVCODEC_INCLUDE_DIR
            NAMES libavcodec/avcodec.h
            PATHS 
                ${FFMPEG_ROOT}/include
                ${FFMPEG_ROOT}
                ${FFMPEG_ROOT}/ffmpeg/include
                ${FFMPEG_ROOT}/src/include
                ${FFMPEG_ROOT}/include/ffmpeg
                ${FFMPEG_ROOT}/../include
                C:/ffmpeg/include
            NO_DEFAULT_PATH
        )
        
        # Si pas trouvé, chercher récursivement
        if(NOT FFMPEG_AVCODEC_INCLUDE_DIR)
            message(STATUS "Trying recursive search for avcodec.h...")
            file(GLOB_RECURSE FFMPEG_HEADERS "${FFMPEG_ROOT}/*avcodec.h")
            message(STATUS "Found headers: ${FFMPEG_HEADERS}")
        endif()
        
        # Chercher les libs
        find_library(FFMPEG_AVCODEC_LIBRARY
            NAMES avcodec libavcodec
            PATHS 
                ${FFMPEG_ROOT}/lib
                ${FFMPEG_ROOT}/lib/x64
                ${FFMPEG_ROOT}/bin
                ${FFMPEG_ROOT}
            NO_DEFAULT_PATH
        )
        
        find_library(FFMPEG_AVFORMAT_LIBRARY
            NAMES avformat libavformat
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64 ${FFMPEG_ROOT}
            NO_DEFAULT_PATH
        )
        
        find_library(FFMPEG_AVUTIL_LIBRARY
            NAMES avutil libavutil
            PATHS ${FFMPEG_ROOT}/lib ${FFMPEG_ROOT}/lib/x64 ${FFMPEG_ROOT}
            NO_DEFAULT_PATH
        )
        
        if(FFMPEG_AVCODEC_INCLUDE_DIR AND FFMPEG_AVCODEC_LIBRARY)
            message(STATUS "✅ Found FFmpeg include: ${FFMPEG_AVCODEC_INCLUDE_DIR}")
            message(STATUS "✅ Found FFmpeg library: ${FFMPEG_AVCODEC_LIBRARY}")
            
            add_library(FFMPEG::avcodec UNKNOWN IMPORTED)
            set_target_properties(FFMPEG::avcodec PROPERTIES
                IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
            )
            
            if(FFMPEG_AVFORMAT_LIBRARY)
                add_library(FFMPEG::avformat UNKNOWN IMPORTED)
                set_target_properties(FFMPEG::avformat PROPERTIES
                    IMPORTED_LOCATION "${FFMPEG_AVFORMAT_LIBRARY}"
                    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
                )
            endif()
            
            if(FFMPEG_AVUTIL_LIBRARY)
                add_library(FFMPEG::avutil UNKNOWN IMPORTED)
                set_target_properties(FFMPEG::avutil PROPERTIES
                    IMPORTED_LOCATION "${FFMPEG_AVUTIL_LIBRARY}"
                    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_AVCODEC_INCLUDE_DIR}"
                )
            endif()
        else()
            message(WARNING "❌ FFmpeg not found in ${FFMPEG_ROOT}")
            message(STATUS "Will try to build without FFmpeg (stub mode)")
            
            # Créer des stubs pour continuer
            add_library(FFMPEG::avcodec INTERFACE IMPORTED)
            add_library(FFMPEG::avformat INTERFACE IMPORTED)
            add_library(FFMPEG::avutil INTERFACE IMPORTED)
        endif()
    else()
        message(WARNING "❌ FFMPEG_DIR environment variable not set")
        # Créer des stubs
        add_library(FFMPEG::avcodec INTERFACE IMPORTED)
        add_library(FFMPEG::avformat INTERFACE IMPORTED)
        add_library(FFMPEG::avutil INTERFACE IMPORTED)
    endif()
    
    # MPV
    if(DEFINED ENV{MPV_DIR} AND EXISTS "$ENV{MPV_DIR}")
        set(MPV_ROOT "$ENV{MPV_DIR}")
        message(STATUS "Looking for MPV in: ${MPV_ROOT}")
        
        # Afficher le contenu
        file(GLOB MPV_CONTENTS "${MPV_ROOT}/*")
        message(STATUS "MPV directory contents: ${MPV_CONTENTS}")
        
        find_path(MPV_INCLUDE_DIR
            NAMES mpv/client.h client.h
            PATHS 
                ${MPV_ROOT}/include
                ${MPV_ROOT}/include/mpv
                ${MPV_ROOT}/mpv
                ${MPV_ROOT}
            NO_DEFAULT_PATH
        )
        
        find_library(MPV_LIBRARY
            NAMES mpv mpv-1 libmpv
            PATHS 
                ${MPV_ROOT}/lib 
                ${MPV_ROOT}/lib/x64 
                ${MPV_ROOT}
                ${MPV_ROOT}/bin
            NO_DEFAULT_PATH
        )
        
        if(MPV_INCLUDE_DIR AND MPV_LIBRARY)
            message(STATUS "✅ Found MPV include: ${MPV_INCLUDE_DIR}")
            message(STATUS "✅ Found MPV library: ${MPV_LIBRARY}")
            
            add_library(MPV::libmpv UNKNOWN IMPORTED)
            set_target_properties(MPV::libmpv PROPERTIES
                IMPORTED_LOCATION "${MPV_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${MPV_INCLUDE_DIR}"
            )
        else()
            message(WARNING "❌ MPV not found in ${MPV_ROOT}")
            add_library(MPV::libmpv INTERFACE IMPORTED)
        endif()
    else()
        message(WARNING "❌ MPV_DIR environment variable not set")
        add_library(MPV::libmpv INTERFACE IMPORTED)
    endif()
    
    # Lier les bibliothèques (avec vérification d'existence)
    target_link_libraries(OptiPlayerCore
        PRIVATE
            fmt::fmt
            spdlog::spdlog
            nlohmann_json::nlohmann_json
    )
    
    # Ajouter FFmpeg seulement si trouvé
    if(TARGET FFMPEG::avcodec AND NOT "${FFMPEG_AVCODEC_LIBRARY}" STREQUAL "FFMPEG_AVCODEC_LIBRARY-NOTFOUND")
        target_link_libraries(OptiPlayerCore PRIVATE FFMPEG::avcodec)
    endif()
    if(TARGET FFMPEG::avformat AND NOT "${FFMPEG_AVFORMAT_LIBRARY}" STREQUAL "FFMPEG_AVFORMAT_LIBRARY-NOTFOUND")
        target_link_libraries(OptiPlayerCore PRIVATE FFMPEG::avformat)
    endif()
    if(TARGET FFMPEG::avutil AND NOT "${FFMPEG_AVUTIL_LIBRARY}" STREQUAL "FFMPEG_AVUTIL_LIBRARY-NOTFOUND")
        target_link_libraries(OptiPlayerCore PRIVATE FFMPEG::avutil)
    endif()
    
    # Ajouter MPV seulement si trouvé
    if(TARGET MPV::libmpv AND NOT "${MPV_LIBRARY}" STREQUAL "MPV_LIBRARY-NOTFOUND")
        target_link_libraries(OptiPlayerCore PRIVATE MPV::libmpv)
    endif()
    
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
endif()

set_target_properties(OptiPlayerCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
