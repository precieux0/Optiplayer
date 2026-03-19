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
        ${FFMPEG_INCLUDE_DIR}
        ${MPV_INCLUDE_DIR}
)

target_link_libraries(OptiPlayerCore
    PRIVATE
        FFmpeg::avcodec
        FFmpeg::avformat
        FFmpeg::avutil
        FFmpeg::swscale
        MPV::libmpv
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

set_target_properties(OptiPlayerCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
