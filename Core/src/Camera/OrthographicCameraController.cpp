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
    if (Input::IsKeyPressed(GLFW_KEY_A))
      m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;

    if (Input::IsKeyPressed(GLFW_KEY_D))
      m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;

    if (Input::IsKeyPressed(GLFW_KEY_W))
      m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;

    if (Input::IsKeyPressed(GLFW_KEY_S))
      m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;

    m_Camera.SetPosition(m_CameraPosition);
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