cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WebcamCVPostprocess")

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (WIN32)
    message(STATUS "Building for Windows")
    message(STATUS "Available configurations: ${CMAKE_CONFIGURATION_TYPES}")
    if (${CMAKE_CONFIGURATION_TYPES} STREQUAL "Release")
        message(STATUS "Building in Release mode")
    endif()

elseif(UNIX)
    message(STATUS "Building for Unix")
    message(STATUS "Current configuration: ${CMAKE_BUILD_TYPE}")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "Building in Release mode")
    endif()

    elseif(APPLE)
        message(FATAL_ERROR "MacOS is not supported!!!")
endif()

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UTILITIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)
find_package(OpenGL REQUIRED)

# Get glew
set(glew-cmake_BUILD_SHARED FALSE)
set(ONLY_LIBS ON)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)
FetchContent_MakeAvailable(glew)

# Get GLFW from Cherno
set(GLFW_BUILD_TESTS FALSE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/TheCherno/glfw.git
		GIT_TAG origin/master
)
FetchContent_MakeAvailable(glfw)

# Get Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG "8ba7efb"
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_PATH ${CMAKE_BINARY_DIR}/_deps/imgui-src)
    file(GLOB IMGUI_GLOB
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imconfig.h
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_internal.h
        ${IMGUI_PATH}/imstb_rectpack.h
        ${IMGUI_PATH}/imstb_textedit.h
        ${IMGUI_PATH}/imstb_truetype.h
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp

        # specific bindings...
        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)
    add_library("imgui" STATIC ${IMGUI_GLOB})
    target_include_directories("imgui" PUBLIC ${IMGUI_PATH})
    target_link_libraries("imgui" PUBLIC glfw)
    endif()

find_package( OpenCV REQUIRED HINTS $ENV{OpenCV_DIR})

if (WIN32)
    if (MSVC AND ${CMAKE_CONFIGURATION_TYPES} STREQUAL "Release")
        add_executable(${PROJECT_NAME} WIN32
            "src/main.cpp" "src/Roboto-Medium.cpp" "src/Application.cpp"
            "include/Application.h"
            "include/WebcamCapture.h" "src/WebcamCapture.cpp" "include/Renderer.h" "src/Renderer.cpp"
            "include/PostProcessingPipeline.h" "src/PostProcessingPipeline.cpp" "include/Effect.h" "include/NegativeEffect.h" "utils/Event.h" "include/Controller.h" "src/Controller.cpp" "include/GrayscaleEffect.h" "include/BlurEffect.h" "include/EdgesEffect.h" "include/ContourEffect.h" "include/TriangulationEffect.h" "include/DenoisingEffect.h" "include/AcidEffect.h"
        "include/ChromaticAberrationEffect.h"
        "utils/Shader.h" "utils/Shader.cpp" "utils/IndexBuffer.h" "utils/IndexBuffer.cpp"
        "utils/VertexBuffer.h" "utils/VertexBuffer.cpp" "utils/VertexArray.h" "utils/VertexArray.cpp"
        "utils/VertexBufferLayout.h" "utils/Framebuffer.h" "utils/Framebuffer.cpp"
        )
    
    else()
        add_executable(${PROJECT_NAME}
        "src/main.cpp" "src/Roboto-Medium.cpp" "src/Application.cpp"
            "include/Application.h"
            "include/WebcamCapture.h" "src/WebcamCapture.cpp" "include/Renderer.h" "src/Renderer.cpp"
            "include/PostProcessingPipeline.h" "src/PostProcessingPipeline.cpp" "include/Effect.h" "include/NegativeEffect.h" "utils/Event.h" "include/Controller.h" "src/Controller.cpp" "include/GrayscaleEffect.h" "include/BlurEffect.h" "include/EdgesEffect.h" "include/ContourEffect.h" "include/TriangulationEffect.h" "include/DenoisingEffect.h" "include/AcidEffect.h"
        "include/ChromaticAberrationEffect.h"
        "utils/Shader.h" "utils/Shader.cpp" "utils/IndexBuffer.h" "utils/IndexBuffer.cpp"
        "utils/VertexBuffer.h" "utils/VertexBuffer.cpp" "utils/VertexArray.h" "utils/VertexArray.cpp"
        "utils/VertexBufferLayout.h" "utils/Framebuffer.h" "utils/Framebuffer.cpp"
    )
    endif()
else()
    add_executable(${PROJECT_NAME}
        "src/main.cpp" "src/Roboto-Medium.cpp" "src/Application.cpp"
            "include/Application.h"
            "include/WebcamCapture.h" "src/WebcamCapture.cpp" "include/Renderer.h" "src/Renderer.cpp"
            "include/PostProcessingPipeline.h" "src/PostProcessingPipeline.cpp" "include/Effect.h" "include/NegativeEffect.h" "utils/Event.h" "include/Controller.h" "src/Controller.cpp" "include/GrayscaleEffect.h" "include/BlurEffect.h" "include/EdgesEffect.h" "include/ContourEffect.h" "include/TriangulationEffect.h" "include/DenoisingEffect.h" "include/AcidEffect.h"
        "include/ChromaticAberrationEffect.h"
        "utils/Shader.h" "utils/Shader.cpp" "utils/IndexBuffer.h" "utils/IndexBuffer.cpp"
        "utils/VertexBuffer.h" "utils/VertexBuffer.cpp" "utils/VertexArray.h" "utils/VertexArray.cpp"
        "utils/VertexBufferLayout.h" "utils/Framebuffer.h" "utils/Framebuffer.cpp"
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE imgui ${GLEW_INCLUDE_DIRS} ${INCLUDE_DIR} ${UTILITIES_DIR} ${OpenCV_INCLUDE_DIRS})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui libglew_static ${OpenCV_LIBS} $<$<CONFIG:Release>:Kernel32.lib>)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui libglew_static ${OpenCV_LIBS})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG_BUILD> $<$<CONFIG:Release>:RELEASE_BUILD>)