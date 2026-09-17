#pragma once

#include <memory>

#include "Window.h"
#include "Event/ApplicationEvent.h"

namespace PicoEngine
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event &event);

    Window &GetWindow() { return *m_Window; }
    static Application &Get();

  protected:
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}

    virtual void OnAppEvent(Event &event) {}

  private:
    bool OnWindowClose(WindowCloseEvent &event);
    bool OnWindowResize(WindowResizeEvent &event);

  private:
    std::unique_ptr<Window> m_Window;

    bool m_Running = true;
    bool m_Minimized = false;

    float m_LastFrameTime = 0.0f;

    static Application *s_Instance;
  };
}
