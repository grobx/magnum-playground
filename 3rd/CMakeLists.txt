### GLFW ###
add_subdirectory(glfw EXCLUDE_FROM_ALL)

### IMGUI ###
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

### CORRADE ###
add_subdirectory(corrade EXCLUDE_FROM_ALL)

### MAGNUM ###
add_subdirectory(magnum EXCLUDE_FROM_ALL)

### MAGNUM INTEGRATION ###
add_subdirectory(magnum-integration EXCLUDE_FROM_ALL)
