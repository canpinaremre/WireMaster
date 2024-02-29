cmake_minimum_required(VERSION 3.5.0)

project(WireMaster VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


add_subdirectory(libs/glfw)


include_directories(libs/imgui)
include_directories(libs/imgui/backends)
include_directories(libs/imgui_node_editor)
include_directories(libs/imgui_node_editor/external/stb_image)
include_directories(libs/imgui_node_editor/ScopeGuard)
include_directories(src)
include_directories(src/Application)



# file(COPY "libs/imgui_node_editor/examples/application/include/application.h" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/application.cpp" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/setup.h" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/renderer.h" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/renderer_ogl3.cpp" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/platform.h" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)
# file(COPY "libs/imgui_node_editor/examples/application/source/platform_glfw.cpp" DESTINATION ${PROJECT_SOURCE_DIR}/src/Application)


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(APPEND SOURCES
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui.cpp
)

list(APPEND SOURCES
    libs/imgui_node_editor/crude_json.cpp
    libs/imgui_node_editor/imgui_canvas.cpp
    libs/imgui_node_editor/imgui_node_editor_api.cpp
    libs/imgui_node_editor/imgui_node_editor.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
#   "glfw"
#   "opengl32"
#   "imm32"


file(COPY resources DESTINATION ${PROJECT_SOURCE_DIR}/bin)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    BACKEND_CONFIG=IMGUI_GLFW
    RENDERER_CONFIG=IMGUI_OGL3
)