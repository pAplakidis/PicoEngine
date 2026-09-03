#include "Input.h"

namespace PicoEngine
{
  GLFWwindow *Input::s_Window = nullptr;

  void Input::Init(GLFWwindow *window)
  {
    s_Window = window;
  }

  bool Input::IsKeyPressed(int keycode)
  {
    int state = glfwGetKey(s_Window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::IsMouseButtonPressed(int button)
  {
    int state = glfwGetMouseButton(s_Window, button);
    return state == GLFW_PRESS;
  }

  float Input::GetMouseX()
  {
    double xpos;
    glfwGetCursorPos(s_Window, &xpos, nullptr);
    return static_cast<float>(xpos);
  }

  float Input::GetMouseY()
  {
    double ypos;
    glfwGetCursorPos(s_Window, nullptr, &ypos);
    return static_cast<float>(ypos);
  }
}
