#include "Application.h"

#include <GL/glew.h>

#include "Core/src/Layer.h"
#include "Core/src/Renderer/RendererCommand.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestBatchRenderer2D.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTransformations.h"
#include "tests/TestTriangle.h"

class SandboxLayer : public PicoEngine::Layer
{
public:
  SandboxLayer()
      : Layer("SandboxLayer")
  {
  }

  void OnAttach() override
  {
    GLFWwindow *window = static_cast<GLFWwindow *>(
        PicoEngine::Application::Get()
            .GetWindow()
            .GetNativeWindow());

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450 core");
    ImGui::StyleColorsDark();

    m_TestMenu = new test::TestMenu(m_CurrentTest);
    m_CurrentTest = m_TestMenu;

    m_TestMenu->RegisterTest<test::TestClearColor>("Clear Color");
    m_TestMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    m_TestMenu->RegisterTest<test::TestTriangle>("Triangle");
    m_TestMenu->RegisterTest<test::TestBatchRenderer2D>("2D Batch Stress Test");
    m_TestMenu->RegisterTest<test::TestTransformations>("Transformations Test");
  }

  void OnDetach() override
  {
    if (m_CurrentTest != m_TestMenu)
      delete m_CurrentTest;

    delete m_TestMenu;

    m_CurrentTest = nullptr;
    m_TestMenu = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void OnUpdate(float deltaTime) override
  {
    if (m_CurrentTest)
      m_CurrentTest->OnUpdate(deltaTime);

    if (m_CurrentTest)
      m_CurrentTest->OnRender();
  }

  void OnImGuiRender() override
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_CurrentTest)
    {
      ImGui::Begin("Test");

      if (m_CurrentTest != m_TestMenu &&
          ImGui::Button("<-"))
      {
        delete m_CurrentTest;
        m_CurrentTest = m_TestMenu;
      }

      m_CurrentTest->OnImGuiRender();

      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void OnEvent(PicoEngine::Event &event) override
  {
    if (m_CurrentTest)
      m_CurrentTest->OnEvent(event);
  }

private:
  test::Test *m_CurrentTest = nullptr;
  test::TestMenu *m_TestMenu = nullptr;
};

class SandboxApplication : public PicoEngine::Application
{
public:
  SandboxApplication()
  {
    PushLayer(new SandboxLayer());
  }
};

int main()
{
  PicoEngine::Log::Init();

  SandboxApplication app;
  app.Run();

  return 0;
}
