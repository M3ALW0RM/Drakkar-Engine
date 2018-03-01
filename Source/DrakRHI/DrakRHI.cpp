#include <DrakRHI/DrakVideo.h>

using namespace drak::gfx;

int main() {
	DrakGLWindow*	win;
	DrakGLContext*	ctx;

	if (DrakVideo::Startup()) {
		win = DrakVideo::CreateGLWindow(640, 640, "TestWindow");
		ctx = DrakVideo::CreateGLContext(win);

		return 0;
	}
    return 1;
}