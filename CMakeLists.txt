cmake_minimum_required(VERSION 3.25)
project(magnum-playground)

### LANGUAGES
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

### OPTIONS
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_IMGUI ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_OBJIMPORTER ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_EIGEN ON CACHE BOOL "" FORCE)

### CMAKE MODULES
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/3rd/magnum/modules/")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/3rd/magnum-integration/modules/")
add_subdirectory(3rd)
message(${CMAKE_MODULE_PATH})

### FIND DEPENDENCIES
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(Corrade REQUIRED)
find_package(Magnum REQUIRED GL Shaders Sdl2Application)
find_package(MagnumIntegration REQUIRED ImGui Eigen)

### ADD TARGET
set(TARGET_NAME playground)
add_executable(${TARGET_NAME} src/main.cpp src/playground.cpp)
target_compile_definitions(${TARGET_NAME} PRIVATE MAGNUM_IMGUIINTEGRATION_USER_CONFIG="imconfig.h")
target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
    Eigen3::Eigen
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
    MagnumIntegration::ImGui)
