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

if(WIN32)
    target_link_libraries(OptiPlayerCore
        PRIVATE
            FFMPEG::avcodec
            MPV::libmpv
            fmt::fmt
            spdlog::spdlog
            nlohmann_json::nlohmann_json
    )
else()
    target_link_libraries(OptiPlayerCore
        PRIVATE
            PkgConfig::FFMPEG
            PkgConfig::MPV
            fmt::fmt
            spdlog::spdlog
            nlohmann_json::nlohmann_json
    )
endif()

# Propriétés
set_target_properties(OptiPlayerCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
