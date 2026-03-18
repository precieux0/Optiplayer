add_library(OptiPlayerCore STATIC
    player.cpp
    decoder.cpp
    syncing.cpp
)

target_link_libraries(OptiPlayerCore
    PkgConfig::FFMPEG
    PkgConfig::MPV
)

target_include_directories(OptiPlayerCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
