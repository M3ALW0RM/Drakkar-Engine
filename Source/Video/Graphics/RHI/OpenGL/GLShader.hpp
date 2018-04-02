#pragma once

#include <unordered_map>
#include <vector>

#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Matrix4x4.hpp>
#include <Video/Graphics/RHI/OpenGL/GLObject.hpp>

namespace drak {
namespace video {
namespace gl {

/*!
* \class GLShader
* \ingroup DrakVideo
* \brief
*
*/
class GLShader : GLObject
{
	using UniformMap = std::unordered_map<std::string, GLint>;

public:
	GLShader() = default;
	virtual ~GLShader();

	bool loadFromData(const std::string& vertCode, const std::string& fragCode);
	bool loadFromFile(const std::string& vertFilename, const std::string& fragFilename);

	void use() const;

	void setUniform(const std::string& name, GLfloat value);
	void setUniform(const std::string& name, GLint value);
	void setUniform(const std::string& name, GLuint value);

	void setUniform(const std::string& name, const math::Vec2f& v2);
	void setUniform(const std::string& name, const math::Vec3f& v3);
	void setUniform(const std::string& name, const math::Vec4f& v4);
	void setUniform(const std::string& name, const math::Mat4f& matrix);

private:
	bool readFileText(const std::string& filename, std::vector<char>& text);
	bool compileProgram(const char* vertCode, const char* fragCode);
	bool compileShader(GLuint shaderID, const char* code);

private:
	UniformMap	m_uniMap;
};

} // namespace gl
} // namespace gfx
} // namespace drak
