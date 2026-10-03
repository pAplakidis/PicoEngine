#include "Application.h"

#include "Renderer/RendererCommand.h"
#include "Util/Log.h"

class CaptainClawApplication : public PicoEngine::Application
{
protected:
  void OnRender() override
  {
    PicoEngine::RendererCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    PicoEngine::RendererCommand::Clear();
  }
};

int main()
{
  PicoEngine::Log::Init();
  CaptainClawApplication app;
  app.Run();
  return 0;
}
