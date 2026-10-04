#include "Application.h"

#include <GL/glew.h>

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"
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

    LOG_CORE_TRACE("{0}", event.ToString());

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

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
      (*--it)->OnEvent(event);
      if (event.IsHandled())
        break;
    }
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
        PicoEngine::RendererCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        PicoEngine::RendererCommand::Clear();

        for (Layer *layer : m_LayerStack)
          layer->OnUpdate(dt);

        for (Layer *layer : m_LayerStack)
          layer->OnRender();

        for (Layer *layer : m_LayerStack)
          layer->OnImGuiRender();
      }

      m_Window->OnUpdate();
    }
  }

  void Application::PushLayer(Layer *layer)
  {
    m_LayerStack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer *overlay)
  {
    m_LayerStack.PushOverlay(overlay);
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
