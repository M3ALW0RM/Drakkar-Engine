#pragma once
#include <Core/Core.hpp>
#include <Core/Timer/FrameTimer.hpp>
#include <Threading/Thread/ThreadPool.hpp>
#include <Engine/Physics/PhysicsSystem.hpp>
#include <Engine/Scene/SceneSystem.hpp>
#include <Video/VideoSystem.hpp>
#include <Video/Graphics/Rendering/RenderSystem.hpp>

namespace drak {
class IManualSceneBlueprint;

namespace core {

class Engine final {
	DK_NONMOVABLE_NONCOPYABLE(Engine)

public:
	DRAK_API int startup();
	DRAK_API int shutdown();
	DRAK_API void startLoop();
	DRAK_API static void stopGame();
	DRAK_API void loadScene(IManualSceneBlueprint& sceneBluePrint);
	DRAK_API static Engine& Get() {
		static Engine engine;
		return engine;
	}
	DRAK_API ~Engine();
	DRAK_API PhysicsSystem& getPhysicsSystem();
private:
	DRAK_API Engine();
	time::FrameTimer	s_frameTime;
	PhysicsSystem		physicsSystem;
	SceneSystem			sceneSystem;
	video::VideoSystem	videoSystem;
	gfx::RenderSystem	renderSystem;
	video::AWindow*		pMainWindow;

	DRAK_API static thread::ThreadPool s_pool;
	DRAK_API static bool running;
};

} // namespace core
} // namespace drak