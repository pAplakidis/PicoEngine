#pragma once

#include <GL/glew.h>

#include "Log.h"

#ifdef PICOENGINE_DEBUG
#define PICOENGINE_ENABLE_ASSERTS
#endif

#if defined(_MSC_VER)
#define PICOENGINE_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define PICOENGINE_DEBUGBREAK() __builtin_trap()
#else
#error "Unsupported compiler for PICOENGINE_DEBUGBREAK"
#endif

#ifdef PICOENGINE_ENABLE_ASSERTS
#define PICOENGINE_ASSERT(x, ...)                      \
  do                                                   \
  {                                                    \
    if (!(x))                                          \
    {                                                  \
      LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      PICOENGINE_DEBUGBREAK();                         \
    }                                                  \
  } while (false)
#else
#define PICOENGINE_ASSERT(x, ...)
#endif

namespace PicoEngine
{

  enum class DebugLogLevel
  {
    None = 0,
    HighAssert = 1,
    High = 2,
    Medium = 3,
    Low = 4,
    Notification = 5
  };

  void EnableGLDebugging();
  void SetGLDebugLogLevel(DebugLogLevel level);
  void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

}
