#pragma once

#include "glm/glm.hpp"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"

#include "OrthographicCamera.h"

namespace PicoEngine
{

  class OrthographicCameraController
  {
  public:
    OrthographicCameraController(float width, float height);

    void OnUpdate(float deltaTime);
    void OnEvent(Event &event);

    OrthographicCamera &GetCamera() { return m_Camera; }

    void SetZoomLevel(float zoomLevel);
    float GetZoomLevel() const { return m_ZoomLevel; }

  private:
    bool OnMouseScrolled(MouseScrolledEvent &event);
    bool OnWindowResized(WindowResizeEvent &event);

    void RecalculateProjection();

  private:
    float m_ViewportWidth, m_ViewportHeight;

    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;

    OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition{0.0f};

    bool m_IsPanning = false;
    float m_PanSpeed = 1.0f;
    glm::vec2 m_LastMousePosition{0.0f};
  };

}