#include <GL/glew.h>
#include <Video/Windowing/SDLRenderWindow.hpp>

namespace drak {
namespace video {

SDLRenderWindow::SDLRenderWindow(const WindowSettings& settings)
:	ARenderWindow(settings) {

	U32 winFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_pWin = SDL_CreateWindow(
		settings.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		settings.resX,
		settings.resY,
		winFlags
	);

	SDL_GL_CreateContext((SDL_Window*)m_pWin);
	m_open = true;
}

SDLRenderWindow::~SDLRenderWindow() {
	if (m_open) Close();
}

void SDLRenderWindow::PollEvents() {
	SDL_PollEvent(&m_evt);
	switch (m_evt.type) {
	case SDL_WINDOWEVENT:
		m_open = m_evt.window.event != SDL_WINDOWEVENT_CLOSE;
		break;
	}
}

void SDLRenderWindow::Clear() {
	F32 color[] = { 0.f, 0.f, 0.f };
	glClearBufferfv(GL_COLOR, 0, color);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void SDLRenderWindow::SwapBuffers() {
	SDL_GL_SwapWindow((SDL_Window*)m_pWin);
}

void SDLRenderWindow::Close() {
	SDL_DestroyWindow((SDL_Window*)m_pWin);
}

} // namespace video
} // namespace drak

