#include "Window.h"

#include <cstdlib>

#include "Util/Log.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace PicoEngine
{
  Window::Window()
      : Window(Props{})
  {
  }

  Window::Window(const Props &props)
      : m_Data{
            props.Title,
            props.Width,
            props.Height,
            false}
  {
    Init(props);
  }

  Window::~Window()
  {
    Shutdown();
  }

  void Window::OnUpdate()
  {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
  }

  uint32_t Window::GetWidth() const { return m_Data.Width; }

  uint32_t Window::GetHeight() const { return m_Data.Height; }

  void Window::SetEventCallback(const EventCallbackFn &callback) { m_Data.EventCallback = callback; }

  void Window::SetVSync(bool enabled)
  {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
  }

  bool Window::IsVSync() const { return m_Data.VSync; }

  void *Window::GetNativeWindow() const { return m_Window; }

  void Window::Init(const Props &props)
  {

    if (!glfwInit())
    {
      LOG_CORE_ERROR("Error initializing GLFW");
      std::exit(1); // TODO: exit and error codes
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef PICOENGINE_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // FIXME: use dynamic width and height for Camera, etc as well
    m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
      LOG_CORE_ERROR("Error creating GLFW window");
      glfwTerminate();
      std::exit(1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetWindowSizeCallback(
        m_Window,
        [](GLFWwindow *window, int width, int height)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
          data.Width = width;
          data.Height = height;
        });

    glfwSetFramebufferSizeCallback(
        m_Window,
        [](GLFWwindow *window, int width, int height)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
          WindowResizeEvent event(width, height);
          if (data.EventCallback)
            data.EventCallback(event);
        });

    glfwSetWindowCloseCallback(
        m_Window,
        [](GLFWwindow *window)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
          WindowCloseEvent event;
          data.EventCallback(event);
        });

    glfwSetKeyCallback(
        m_Window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

          switch (action)
          {
          case GLFW_PRESS:
          {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
          }

          case GLFW_RELEASE:
          {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
          }

          case GLFW_REPEAT:
          {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
          }
          }
        });

    glfwSetMouseButtonCallback(
        m_Window,
        [](GLFWwindow *window, int button, int action, int mods)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

          if (action == GLFW_PRESS)
          {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
          }
          else if (action == GLFW_RELEASE)
          {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
          }
        });

    glfwSetScrollCallback(
        m_Window,
        [](GLFWwindow *window, double xOffset, double yOffset)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
          MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
          data.EventCallback(event);
        });

    glfwSetCursorPosCallback(
        m_Window,
        [](GLFWwindow *window, double x, double y)
        {
          auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
          MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
          data.EventCallback(event);
        });
  }

  void Window::Shutdown()
  {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    m_Window = nullptr;
  }
}
