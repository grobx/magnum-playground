### SDL2
add_subdirectory(SDL2 EXCLUDE_FROM_ALL)
find_package(SDL2 REQUIRED)

### IMGUI
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

### EIGEN
#add_subdirectory(eigen EXCLUDE_FROM_ALL)

### CORRADE
add_subdirectory(corrade EXCLUDE_FROM_ALL)

### MAGNUM
add_subdirectory(magnum EXCLUDE_FROM_ALL)

### MAGNUM INTEGRATION
add_subdirectory(magnum-integration EXCLUDE_FROM_ALL)
