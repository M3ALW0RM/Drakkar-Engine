#pragma once

#include "SDL_events.h"

#include <Windowing/ARenderWindow.hpp>

namespace drak {
namespace video {

/*!
* \class SDLRenderWindow
* \ingroup DrakVideo
* \brief Inherits ARenderWindow; builds on an SDL_Window
*
*/
class SDLRenderWindow final : public ARenderWindow {
public:
	SDLRenderWindow(const WindowSettings& settings);
	virtual ~SDLRenderWindow();

private:
	static bool InitSDLVideo();
	static void QuitSDLVideo();

private:
	void PollEvents()	override;
	void Clear()		override;
	void SwapBuffers()	override;
	void Close()		override;

private:
	void*		m_glContext;
	SDL_Event	m_evt;

friend bool VideoSystem::startup(const VideoSettings& settings);
friend void VideoSystem::shutdown();
};

} // namespace video
} // namespace drak