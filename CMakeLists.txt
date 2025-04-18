cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(GStreamerExample VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 查找 GStreamer 库
find_package(PkgConfig REQUIRED)

# 查找 GStreamer 库
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_RTSP REQUIRED gstreamer-rtsp-server-1.0)

# 包含 GStreamer 的头文件路径
include_directories(${GSTREAMER_INCLUDE_DIRS} ${GSTREAMER_RTSP_INCLUDE_DIRS})

# 添加源代码
add_executable(gstreamer_example src/h265_rtsp.cpp)

# 链接 GStreamer 和 RTSP server 库
target_link_libraries(gstreamer_example ${GSTREAMER_LIBRARIES} ${GSTREAMER_RTSP_LIBRARIES})

# 显示 GStreamer 库的路径（可选）
message(STATUS "GStreamer Include Directories: ${GSTREAMER_INCLUDE_DIRS}")
message(STATUS "GStreamer Libraries: ${GSTREAMER_LIBRARIES}")