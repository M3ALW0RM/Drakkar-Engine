#include <Core/Engine/Engine.hpp>
#include <Core/Components/AGameObject.h>
#include <Core/Scene/SceneSystem.h>
#include <Core/Timer/FrameTimer.hpp>
namespace drak {

	time::FrameTimer& core::Engine::s_frameTime = time::FrameTimer();
namespace core {
	bool Engine::running = true;

	int Engine::startup()
	{
		Logbook::Log(Logbook::EOutput::CONSOLE, "EngineLog.txt", "Init systems\n");
		//Init systems
		SceneSystem::Startup();
		s_frameTime.start();
		return 0;
	}

	int Engine::shutdown()
	{
		Logbook::Log(Logbook::EOutput::CONSOLE, "EngineLog.txt", "Shutdown systems\n");
		SceneSystem::Shutdown();
		Logbook::CloseLogs();
		return 0;
	}

	void Engine::startLoop()
	{
		Scene* scene = SceneSystem::GetScene();
		std::list<AGameObject*> gameObjects = scene->GetGameObjects();
		std::list<AGameObject*>::iterator it = gameObjects.begin();
		while (it != gameObjects.end())
		{
			(*it)->Start();
			std::advance(it, 1);
		}
		while (running)
		{
			s_frameTime.update();
			Logbook::Log(Logbook::EOutput::CONSOLE, "EngineLog.txt", "DeltaTime: %f\n",s_frameTime.deltaTime());
			std::list<AGameObject*> gameObjects = scene->GetGameObjects();
			std::list<AGameObject*>::iterator it = gameObjects.begin();
			for (unsigned int i = 0; i < gameObjects.size(); ++i)
			{
				(*it)->Update();
				std::advance(it, 1);
			}
		}
	}

 void Engine::stopGame()
	{
		Engine::running = false;
	}

} // namespace core
} // namespace drak