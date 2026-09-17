#include "Application.h"

#include <GL/glew.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestBatchRenderer2D.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTransformations.h"
#include "tests/TestTriangle.h"

class SandboxApplication : public PicoEngine::Application
{
public:
  SandboxApplication()
  {
    GLFWwindow *window = static_cast<GLFWwindow *>(GetWindow().GetNativeWindow());

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

  ~SandboxApplication() override
  {
    if (m_CurrentTest != m_TestMenu)
      delete m_CurrentTest;
    delete m_TestMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

protected:
  void OnUpdate(float deltaTime) override
  {
    if (m_CurrentTest)
      m_CurrentTest->OnUpdate(deltaTime);
  }

  void OnRender() override
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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

  void OnAppEvent(PicoEngine::Event &event) override
  {
    if (m_CurrentTest)
      m_CurrentTest->OnEvent(event);
  }

private:
  test::Test *m_CurrentTest = nullptr;
  test::TestMenu *m_TestMenu = nullptr;
};

int main()
{
  PicoEngine::Log::Init();
  SandboxApplication app;
  app.Run();
  return 0;
}
