#include "OrthographicCameraController.h"

#include <GLFW/glfw3.h>

#include "Util/Input.h"
#include "Util/Log.h"

namespace PicoEngine
{

  OrthographicCameraController::OrthographicCameraController(float width, float height)
      : m_ViewportWidth(width),
        m_ViewportHeight(height),
        m_AspectRatio(width / height),
        m_Camera(0.0f, width, 0.0f, height)
  {
  }

  void OrthographicCameraController::OnUpdate(float deltaTime)
  {
    glm::vec2 mousePosition{
        Input::GetMouseX(),
        Input::GetMouseY()};

    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
      if (!m_IsPanning)
      {
        m_LastMousePosition = mousePosition;
        m_IsPanning = true;
        return;
      }

      glm::vec2 delta = mousePosition - m_LastMousePosition;

      float panScale = m_ZoomLevel;

      m_CameraPosition.x -= delta.x * m_PanSpeed * panScale;
      m_CameraPosition.y += delta.y * m_PanSpeed * panScale;

      m_Camera.SetPosition(m_CameraPosition);

      m_LastMousePosition = mousePosition;
    }
    else
    {
      m_IsPanning = false;
    }
  }

  void OrthographicCameraController::OnEvent(Event &event)
  {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MouseScrolledEvent>(
        [this](MouseScrolledEvent &e)
        {
          return OnMouseScrolled(e);
        });

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent &e)
        {
          return OnWindowResized(e);
        });
  }

  void OrthographicCameraController::SetZoomLevel(float zoomLevel)
  {
    m_ZoomLevel = zoomLevel;
    RecalculateProjection();
  }

  bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &event)
  {
    m_ZoomLevel -= event.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    RecalculateProjection();
    return false;
  }

  bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &event)
  {
    if (event.GetHeight() == 0)
      return false;

    m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
    RecalculateProjection();
    return false;
  }

  void OrthographicCameraController::RecalculateProjection()
  {
    m_Camera.SetProjection(
        0.0f,
        m_ViewportWidth * m_ZoomLevel,
        0.0f,
        m_ViewportHeight * m_ZoomLevel);
  }
}