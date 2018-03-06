#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/Vec4.hpp"
#include "Math/Vec8.hpp"
#include <iostream>

I32 main() {

	drak::math::Vec2i v2i;
	drak::math::Vec2f v2f;

	drak::math::Vec3i v3i;
	drak::math::Vec3f v3f;

	drak::math::Vec4si v4s;
	drak::math::Vec4i v4i;
	drak::math::Vec4f v4f;

	drak::math::Vec8i v8i;
	drak::math::Vec8f v8f;

//////////ADD///////////
	std::cout << "ADDITION :" << "\n";
	v2i += 1;
	v2f += 1;

	v3i += 1;
	v3f += 1;

	v4s += 1;
	v4i += 1;
	v4f += 1;

	v8i += 1;
	v8f += 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i += v2i;
	v2f += v2f;

	v3i += v3i;
	v3f += v3f;

	v4s += v4s;
	v4i += v4i;
	v4f += v4f;

	v8i += v8i;
	v8f += v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i + 1;
	v2f = v2f + 1;

	v3i = v3i + 1;
	v3f = v3f + 1;

	v4s = v4s + 1;
	v4i = v4i + 1;
	v4f = v4f + 1;

	v8i = v8i + 1;
	v8f = v8f + 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i + v2i;
	v2f = v2f + v2f;

	v3i = v3i + v3i;
	v3f = v3f + v3f;

	v4s = v4s + v4s;
	v4i = v4i + v4i;
	v4f = v4f + v4f;

	v8i = v8i + v8i;
	v8f = v8f + v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n\n";

////////////////////////

//////////MUL///////////
	std::cout << "MULTIPLICATION :" << "\n";
	v2i *= 1;
	v2f *= 1;

	v3i *= 1;
	v3f *= 1;

	v4s *= 1;
	v4i *= 1;
	v4f *= 1;

	v8i *= 1;
	v8f *= 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i *= v2i;
	v2f *= v2f;

	v3i *= v3i;
	v3f *= v3f;

	v4s *= v4s;
	v4i *= v4i;
	v4f *= v4f;

	v8i *= v8i;
	v8f *= v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i * 1;
	v2f = v2f * 1;

	v3i = v3i * 1;
	v3f = v3f * 1;

	v4s = v4s * 1;
	v4i = v4i * 1;
	v4f = v4f * 1;

	v8i = v8i * 1;
	v8f = v8f * 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i * v2i;
	v2f = v2f * v2f;

	v3i = v3i * v3i;
	v3f = v3f * v3f;

	v4s = v4s * v4s;
	v4i = v4i * v4i;
	v4f = v4f * v4f;

	v8i = v8i * v8i;
	v8f = v8f * v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n\n";

////////////////////////

//////////SUB///////////
	std::cout << "SUBSTRACTION :" << "\n";
	v2i -= 1;
	v2f -= 1;

	v3i -= 1;
	v3f -= 1;

	v4s -= 1;
	v4i -= 1;
	v4f -= 1;

	v8i -= 1;
	v8f -= 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i -= v2i;
	v2f -= v2f;

	v3i -= v3i;
	v3f -= v3f;

	v4s -= v4s;
	v4i -= v4i;
	v4f -= v4f;

	v8i -= v8i;
	v8f -= v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i - 1;
	v2f = v2f - 1;

	v3i = v3i - 1;
	v3f = v3f - 1;

	v4s = v4s - 1;
	v4i = v4i - 1;
	v4f = v4f - 1;

	v8i = v8i - 1;
	v8f = v8f - 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i - v2i;
	v2f = v2f - v2f;

	v3i = v3i - v3i;
	v3f = v3f - v3f;

	v4s = v4s - v4s;
	v4i = v4i - v4i;
	v4f = v4f - v4f;

	v8i = v8i - v8i;
	v8f = v8f - v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n\n";

////////////////////////

//////////DIV///////////
	std::cout << "DIVISION :" << "\n";
	v2i /= 1;
	v2f /= 1;

	v3i /= 1;
	v3f /= 1;

	v4s /= 1;
	v4i /= 1;
	v4f /= 1;

	v8i /= 1;
	v8f /= 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i /= v2i;
	v2f /= v2f;

	v3i /= v3i;
	v3f /= v3f;

	v4s /= v4s;
	v4i /= v4i;
	v4f /= v4f;

	v8i /= v8i;
	v8f /= v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i / 1;
	v2f = v2f / 1;

	v3i = v3i / 1;
	v3f = v3f / 1;

	v4s = v4s / 1;
	v4i = v4i / 1;
	v4f = v4f / 1;

	v8i = v8i / 1;
	v8f = v8f / 1;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n";

	v2i = v2i / v2i;
	v2f = v2f / v2f;

	v3i = v3i / v3i;
	v3f = v3f / v3f;

	v4s = v4s / v4s;
	v4i = v4i / v4i;
	v4f = v4f / v4f;

	v8i = v8i / v8i;
	v8f = v8f / v8f;

	std::cout << "v2i : " << v2i << "\n";
	std::cout << "v2f : " << v2f << "\n";
	std::cout << "v3i : " << v3i << "\n";
	std::cout << "v3f : " << v3f << "\n";
	std::cout << "v4s : " << v4s << "\n";
	std::cout << "v4i : " << v4i << "\n";
	std::cout << "v4f : " << v4f << "\n";
	std::cout << "v8i : " << v8i << "\n";
	std::cout << "v8f : " << v8f << "\n\n";

////////////////////////
	system("pause");
	return 0;
}


//////////////CHRONO/////////////////
/*auto Start = std::chrono::high_resolution_clock::now();
auto Stop = std::chrono::high_resolution_clock::now();
std::cout << " SSE + : " << std::chrono::duration_cast<std::chrono::milliseconds>(Stop - Start).count() << std::endl;

Start = std::chrono::high_resolution_clock::now();
Stop = std::chrono::high_resolution_clock::now();
std::cout << " naive + : " << std::chrono::duration_cast<std::chrono::milliseconds>(Stop - Start).count() << std::endl;*/