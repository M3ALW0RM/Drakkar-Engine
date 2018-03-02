#include <RHI/OpenGL/OpenGLRHI.hpp>

using namespace drak::gfx;

int main() {
	OpenGLWindow*	win;
	OpenGLContext*	ctx;

	if (OpenGLRHI::Startup()) {
		win = OpenGLRHI::CreateWindow(640, 640, "TestWindow");
		ctx = OpenGLRHI::CreateContext(win);
		return 0;
	}
    return 1;
}