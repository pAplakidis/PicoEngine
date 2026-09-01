#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace PicoEngine
{
  class Log
  {
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetLogger() { return s_Logger; }

  private:
    static std::shared_ptr<spdlog::logger> s_Logger;
  };
}

// Client log macros
#define LOG_TRACE(...) ::PicoEngine::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::PicoEngine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::PicoEngine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::PicoEngine::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::PicoEngine::Log::GetLogger()->critical(__VA_ARGS__)

// Core log macros
#define LOG_CORE_TRACE(...) ::PicoEngine::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::PicoEngine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::PicoEngine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::PicoEngine::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::PicoEngine::Log::GetLogger()->critical(__VA_ARGS__)
