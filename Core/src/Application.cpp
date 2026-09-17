#include "Application.h"

#include "Renderer/Renderer.h"
#include "Util/Input.h"
#include "Util/Log.h"
#include "GLCore/OpenGLDebug.h"

namespace PicoEngine
{
  Application *Application::s_Instance = nullptr;

  Application::Application()
  {
    s_Instance = this;

    m_Window = std::make_unique<Window>();
    m_Window->SetEventCallback(
        [this](Event &event)
        {
          OnEvent(event);
        });

    Input::Init(static_cast<GLFWwindow *>(m_Window->GetNativeWindow()));

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      LOG_CORE_ERROR("Failed to initialize GLEW");
      std::exit(EXIT_FAILURE);
    }

#ifdef PICOENGINE_DEBUG
    EnableGLDebugging();
#endif

    Renderer::Init();

    LOG_INFO("OpenGL: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    LOG_INFO("GLSL: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    m_LastFrameTime = static_cast<float>(glfwGetTime());
  }

  Application::~Application()
  {
  }

  void Application::OnEvent(Event &event)
  {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent &event)
        {
          return OnWindowClose(event);
        });

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent &event)
        {
          return OnWindowResize(event);
        });

    if (!event.IsHandled())
      OnAppEvent(event);
  }

  void Application::Run()
  {
    while (m_Running)
    {
      float time = static_cast<float>(glfwGetTime());
      float dt = time - m_LastFrameTime;
      m_LastFrameTime = time;

      if (!m_Minimized)
      {
        OnUpdate(dt);
        OnRender();

        // TODO: use layers
        // for (Layer* layer : m_LayerStack)
        //   layer->OnUpdate(dt);
        //
        // ImGuiLayer->Begin();
        // for (Layer* layer : m_LayerStack)
        //   layer->OnImGuiRender();
        // ImGuiLayer->End();
      }

      OnImGuiRender();

      m_Window->OnUpdate();
    }
  }

  Application &Application::Get() { return *s_Instance; }

  bool Application::OnWindowClose(WindowCloseEvent &event)
  {
    m_Running = false;
    return true;
  }

  bool Application::OnWindowResize(WindowResizeEvent &event)
  {
    if (event.GetWidth() == 0 || event.GetHeight() == 0)
    {
      m_Minimized = true;
      return false;
    }
    m_Minimized = false;

    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
  }
}
