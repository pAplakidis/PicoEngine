#include <cstdint>
#include <string>
#include <functional>

#include <GLFW/glfw3.h>

#include "Event/Event.h"

namespace PicoEngine
{
  class Window
  {
  public:
    struct Props
    {
      std::string Title = "PicoEngine";
      uint32_t Width = 1280;
      uint32_t Height = 720;
    };

    using EventCallbackFn = std::function<void(Event &)>;

    Window();
    explicit Window(const Props &props);
    ~Window();

    void OnUpdate();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    void SetEventCallback(const EventCallbackFn &callback);
    void SetVSync(bool enabled);
    bool IsVSync() const;

    void *GetNativeWindow() const;

  private:
    void Init(const Props &props);
    void Shutdown();

  private:
    GLFWwindow *m_Window = nullptr;

    struct WindowData
    {
      std::string Title;
      uint32_t Width;
      uint32_t Height;
      bool VSync;

      EventCallbackFn EventCallback;
    };

    WindowData m_Data;
  };
}
