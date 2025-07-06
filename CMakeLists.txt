cmake_minimum_required(VERSION 3.10)
project(VirtualMuseum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

set(GLAD_DIR "${EXTERNAL_DIR}/glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")
set(GLAD_SRC_DIR "${GLAD_DIR}/src")

set(GLFW_DIR "${EXTERNAL_DIR}/glfw/glfw-3.4.bin.WIN64")
set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
set(GLFW_LIB_DIR "${GLFW_DIR}/lib-vc2022")

set(GLM_DIR "${EXTERNAL_DIR}/glm")
set(GLM_INCLUDE_DIR "${GLM_DIR}")

set(ASSIMP_DIR "${EXTERNAL_DIR}/assimp-5.4.3")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_DIR}/include")
set(ASSIMP_BUILD_DIR "${EXTERNAL_DIR}/build")
set(ASSIMP_BUILD_INCLUDE_DIR "${ASSIMP_BUILD_DIR}/include")
set(ASSIMP_LIB_DIR "${ASSIMP_BUILD_DIR}/lib/Debug")
set(ASSIMP_BIN_DIR "${ASSIMP_BUILD_DIR}/bin/Debug")

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

find_package(OpenGL REQUIRED)

file(GLOB ALL_SRC_FILES "src/*.cpp")
list(FILTER ALL_SRC_FILES EXCLUDE REGEX ".*imgui.*\.cpp$")
set(SOURCES ${ALL_SRC_FILES})
list(APPEND SOURCES "${GLAD_SRC_DIR}/glad.c")

if(NOT "${SOURCES}" MATCHES ".*Renderer\.cpp.*")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp")
endif()

if(NOT "${SOURCES}" MATCHES ".*Shader\.cpp.*")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp")
endif()

if(NOT "${SOURCES}" MATCHES ".*StatueManager\.cpp.*")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/StatueManager.cpp")
endif()

if(NOT "${SOURCES}" MATCHES ".*Camera\.cpp.*")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp")
endif()

list(APPEND SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
    ${ASSIMP_BUILD_INCLUDE_DIR}
    ${IMGUI_DIR}
)

add_executable(VirtualMuseum ${SOURCES})

target_link_libraries(VirtualMuseum PRIVATE 
    OpenGL::GL 
    "${GLFW_LIB_DIR}/glfw3.lib"
    "${ASSIMP_LIB_DIR}/assimp-vc143-mtd.lib"
)


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/models)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/textures)

add_custom_command(TARGET VirtualMuseum POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ASSIMP_BIN_DIR}/assimp-vc143-mtd.dll"
    "$<TARGET_FILE_DIR:VirtualMuseum>"
)


add_custom_command(TARGET VirtualMuseum POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_BINARY_DIR}/Debug/models"
    "${CMAKE_BINARY_DIR}/Release/models"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_BINARY_DIR}/models"
    "${CMAKE_BINARY_DIR}/Release/models"
)

add_custom_command(TARGET VirtualMuseum POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_BINARY_DIR}/Debug/textures"
    "${CMAKE_BINARY_DIR}/Release/textures"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_BINARY_DIR}/textures"
    "${CMAKE_BINARY_DIR}/Release/textures"
)
