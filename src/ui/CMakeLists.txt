add_executable(OptiPlayerUI WIN32
    mainwindow.cpp
    main.cpp
)

target_link_libraries(OptiPlayerUI OptiPlayerCore)
target_include_directories(OptiPlayerUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(OptiPlayerUI PROPERTIES
    OUTPUT_NAME "OptiPlayer"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OptiPlayer-Release
)
