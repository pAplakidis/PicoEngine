cmake_minimum_required(VERSION 3.20)

project(PicoEngine
  VERSION 0.1.0
  LANGUAGES C CXX
)

# ---------------------------------------------------------------------------
# C++ configuration
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put built binaries in build/bin instead of scattering them around.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_link_options(-fuse-ld=lld)
endif()


# ---------------------------------------------------------------------------
# OpenGL
# ---------------------------------------------------------------------------

# Windows:
#   resolves to opengl32.lib from the Windows SDK.
#
# Linux:
#   resolves to the system OpenGL library.
find_package(OpenGL REQUIRED)

if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
endif()


# ---------------------------------------------------------------------------
# GLEW
# ---------------------------------------------------------------------------

add_library(glew STATIC
  Dependencies/GLEW/src/glew.c
)

target_include_directories(glew PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include"
)

target_compile_definitions(glew PUBLIC
  GLEW_STATIC
  GLEW_NO_GLU
)

target_link_libraries(glew PUBLIC
  OpenGL::GL
)

if(UNIX AND NOT APPLE)
  target_link_libraries(glew PUBLIC
    X11::X11
    ${CMAKE_DL_LIBS}
  )
endif()


# ---------------------------------------------------------------------------
# GLFW
#
# GLFW is provided precompiled inside Dependencies/.
#
# Windows -> glfw3.lib
# Linux   -> libglfw3.a
# ---------------------------------------------------------------------------

add_library(glfw STATIC IMPORTED GLOBAL)

set_target_properties(glfw PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include"
)

if(WIN32)

  # Prefer a modern Visual Studio build if present.
  if(EXISTS
      "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib")

    set(GLFW_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib"
    )

  # Allows you to keep using Cherno's old directory layout if desired.
  elseif(EXISTS
      "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2015/glfw3.lib")

    set(GLFW_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2015/glfw3.lib"
    )

  else()

    message(FATAL_ERROR
      "Could not find GLFW for Windows.\n"
      "Expected one of:\n"
      "  Dependencies/GLFW/lib-vc2022/glfw3.lib\n"
      "  Dependencies/GLFW/lib-vc2015/glfw3.lib"
    )

  endif()

  set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_LIBRARY}"
  )

  # Required by static GLFW on Windows.
  target_link_libraries(glfw INTERFACE
    gdi32
  )


elseif(UNIX AND NOT APPLE)

  set(GLFW_LIBRARY
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-linux-x86_64/libglfw3.a"
  )

  if(NOT EXISTS "${GLFW_LIBRARY}")
    message(FATAL_ERROR
      "Could not find GLFW for Linux:\n"
      "  ${GLFW_LIBRARY}"
    )
  endif()

  set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_LIBRARY}"
  )

  # Dependencies required by our static X11 GLFW build.
  find_package(Threads REQUIRED)

  target_link_libraries(glfw INTERFACE
    X11::X11
    Threads::Threads
    ${CMAKE_DL_LIBS}
  )


else()

  message(FATAL_ERROR
    "This project currently supports Windows and Linux only."
  )

endif()


# ---------------------------------------------------------------------------
# spdlog
# ---------------------------------------------------------------------------

add_subdirectory(Dependencies/spdlog-1.17.0)


# ---------------------------------------------------------------------------
# Core
# ---------------------------------------------------------------------------

file(GLOB_RECURSE CORE_SOURCES
  CONFIGURE_DEPENDS

  "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.c"
)

file(GLOB_RECURSE CORE_HEADERS
  CONFIGURE_DEPENDS

  "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.hpp"
)

add_library(Core STATIC
  ${CORE_SOURCES}
  ${CORE_HEADERS}
)

target_include_directories(Core
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src"

  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/vendor"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/vendor/stb_image"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/vendor/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/vendor/imgui/backends"
)

# Prevent GLFW from including its own legacy OpenGL headers.
# GLAD owns the OpenGL declarations.
target_compile_definitions(Core
  PUBLIC
    GLFW_INCLUDE_NONE
    "$<$<CONFIG:Debug>:PICOENGINE_DEBUG>"
)

target_compile_definitions(Core
  PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)

target_link_libraries(Core
  PUBLIC
    glfw
    glew
    spdlog::spdlog
    OpenGL::GL
)


# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------

file(GLOB_RECURSE APPLICATION_SOURCES
  CONFIGURE_DEPENDS

  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/tests/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.c"
)

file(GLOB_RECURSE APPLICATION_HEADERS
  CONFIGURE_DEPENDS

  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/tests/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/tests/*.hpp"
)

add_executable(PicoEngine
  ${APPLICATION_SOURCES}
  ${APPLICATION_HEADERS}
)

target_link_libraries(PicoEngine
  PRIVATE
    Core
)

target_include_directories(PicoEngine
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/vendor"
)


# ---------------------------------------------------------------------------
# Resource paths
# ---------------------------------------------------------------------------

# During development, code can use:
#
#   APPLICATION_RESOURCES_PATH
#   CORE_RESOURCES_PATH
#
# Example:
#
#   Shader shader(APPLICATION_RESOURCES_PATH "shaders/basic.shader");

target_compile_definitions(Core
  PUBLIC
    CORE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Core/res/"
)

target_compile_definitions(PicoEngine
  PRIVATE
    APPLICATION_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Application/res/"
)


# ---------------------------------------------------------------------------
# Compiler options
# ---------------------------------------------------------------------------

if(MSVC)

  target_compile_options(Core PRIVATE
    /W4
    /permissive-
  )

  target_compile_options(PicoEngine PRIVATE
    /W4
    /permissive-
  )

  target_compile_definitions(Core PUBLIC
    _CRT_SECURE_NO_WARNINGS
  )

else()

  target_compile_options(Core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )

  target_compile_options(PicoEngine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )

endif()


# ---------------------------------------------------------------------------
# Visual Studio organization
# ---------------------------------------------------------------------------

# Makes the generated Visual Studio solution considerably nicer to navigate.

source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/Core/src"
  PREFIX "Core"
  FILES ${CORE_SOURCES} ${CORE_HEADERS}
)

source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/Application"
  PREFIX "Application"
  FILES ${APPLICATION_SOURCES} ${APPLICATION_HEADERS}
)

# When opening the generated solution in Visual Studio,
# make PicoEngine the default startup project.
set_property(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  PROPERTY VS_STARTUP_PROJECT PicoEngine
)
