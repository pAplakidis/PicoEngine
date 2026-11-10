#pragma once

#include "glm/glm.hpp"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"

#include "OrthographicCamera.h"

namespace PicoEngine
{

  // TODO: rotation
  class OrthographicCameraController
  {
  public:
    OrthographicCameraController(float aspectRatio);

    void OnUpdate(float deltaTime);
    void OnEvent(Event &event);

    OrthographicCamera &GetCamera() { return m_Camera; }

    void SetAspectRatio(float aspectRatio);
    float GetAspectRatio() { return m_AspectRatio; }

    void SetZoomLevel(float zoomLevel);
    float GetZoomLevel() const { return m_ZoomLevel; }

    void setPosition(const glm::vec3 &position)
    {
      m_CameraPosition = position;
      m_Camera.SetPosition(m_CameraPosition);
    }

    glm::vec3 getPosition() const { return m_CameraPosition; }

    void SetPanningEnabled(bool enabled)
    {
      m_PanningEnabled = enabled;
      if (!enabled)
      {
        m_IsPanning = false;
      }
    }

    void SetZoomingEnabled(bool enabled) { m_ZoomingEnabled = enabled; }

  private:
    bool OnMouseScrolled(MouseScrolledEvent &event);
    bool OnWindowResized(WindowResizeEvent &event);

    void RecalculateProjection();

  private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;

    OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition{0.0f};

    bool m_PanningEnabled = true;
    bool m_IsPanning = false;
    bool m_ZoomingEnabled = true;
    float m_PanSpeed = 0.002f;
    glm::vec2 m_LastMousePosition{0.0f};
  };

}