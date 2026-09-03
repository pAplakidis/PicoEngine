// Input.h
#pragma once

#include <GLFW/glfw3.h>

namespace PicoEngine
{
  class Input
  {
  public:
    static void Init(GLFWwindow *window);

    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);

    static float GetMouseX();
    static float GetMouseY();

  private:
    static GLFWwindow *s_Window;
  };
}
