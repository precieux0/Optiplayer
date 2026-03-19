add_executable(OptiPlayerUI WIN32
    main.cpp
    mainwindow.cpp
)

target_include_directories(OptiPlayerUI 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

target_link_libraries(OptiPlayerUI 
    PRIVATE
        OptiPlayerCore
        comctl32
        gdi32
        user32
        kernel32
        ole32
        shell32
        uuid
)

set_target_properties(OptiPlayerUI PROPERTIES
    OUTPUT_NAME "OptiPlayer"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release
)
