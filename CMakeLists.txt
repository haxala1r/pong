cmake_minimum_required(VERSION 3.10)

project(Pong)

find_package(SDL2 REQUIRED)
# cmake cannot find SDL2_ttf with find_package,
# so we rely on find_library to hopefully do it instead.
# I could use a third-party solution - but that would make things inconvenient.
# I could use pkg-config, but that would not be truly cross-platform.
find_library(SDL2_ttf NAMES SDL2_ttf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pong src/main.cpp src/app.cpp src/geometry.cpp)
target_include_directories(pong PRIVATE ${SDL2_INCLUDE_DIRS} "./src/include/")
target_link_libraries(pong ${SDL2_LIBRARIES} ${SDL2_ttf})
