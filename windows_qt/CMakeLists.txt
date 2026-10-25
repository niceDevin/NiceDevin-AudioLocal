cmake_minimum_required(VERSION 3.16)
project(AudioLocalQt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt 路径：由 -DCMAKE_PREFIX_PATH 传入（如 C:/Qt/6.7.3/msvc2019_64）
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/converterpage.cpp
    src/ffmpegworker.cpp
    src/pages.cpp
    src/style.cpp
    resources.qrc
    appicon.rc
)

set(HEADERS
    src/mainwindow.h
    src/converterpage.h
    src/ffmpegworker.h
    src/pages.h
    src/style.h
)

add_executable(AudioLocalQt ${SOURCES} ${HEADERS})

target_link_libraries(AudioLocalQt PRIVATE Qt6::Widgets)

# 安装 ffmpeg.exe 到输出目录
add_custom_command(TARGET AudioLocalQt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/bin/ffmpeg.exe"
        "$<TARGET_FILE_DIR:AudioLocalQt>/ffmpeg.exe"
    COMMENT "Copying ffmpeg.exe next to the executable"
)

set_target_properties(AudioLocalQt PROPERTIES
    WIN32_EXECUTABLE TRUE
)
