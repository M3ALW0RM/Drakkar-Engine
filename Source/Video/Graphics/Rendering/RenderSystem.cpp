#include <Video/Graphics/Rendering/RenderSystem.hpp>
<<<<<<<
#include <Video/Graphics/Rendering/OpenGL/GLTexture.hpp>
=======

>>>>>>>


namespace drak {
namespace gfx {

bool RenderSystem::startup(IRenderer* pRenderer) {
	m_pRenderer = pRenderer;
	m_pRenderer->info();

	m_pRenderer->depthTest(true);
	m_pRenderer->blendTest(true);
	m_pRenderer->cullTest(true);

<<<<<<<
	m_mainCam.view({ 0.f, 10.f, -10.f }, { 0.f, 10.f, 0.f }, { 0.f, 1.f, 0.f });
	m_mainCam.perspective(90.f, 16.f / 9.f, 0.1f, 1000.f);
=======
	m_mainCam.view({ 0.f, 0.f, -10.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
	m_mainCam.perspective(60.f, 16.f / 9.f, 0.1f, 1000.f);
>>>>>>>

<<<<<<<
	gl::GLTexture tex;
	tex.loadFromFile("Resources/Textures/grid_cell.png");
	tex.bind();
	m_gridTex = tex.glID();

=======
	m_gridTex.loadFromFile("Resources/Textures/grid_cell.png");

>>>>>>>
	return loadResources("Resources/");
}

void RenderSystem::shutdown() {
	// ...
	// delete resources
}

bool RenderSystem::loadResources(const std::string& dir) {
	return (m_pRenderer->loadShaders	(dir + "Shaders/", m_shaderMap) &&
			m_pRenderer->loadRenderables(dir + "Models/quad.obj", m_pGrid) &&
			m_pRenderer->loadRenderables(dir + "Models/cube.obj", m_pUnitCube));
}

void RenderSystem::forwardRender(
	std::vector<components::Model>& models,
	std::vector<components::Transform>& xforms) {

	m_shaderMap["DefaultShader"]->use();
	m_shaderMap["DefaultShader"]->setUniform("viewPrsp", m_mainCam.viewPerspective());
	U32 flag = 1 << components::ComponentType<components::Model>::id;
	for (size_t i = 0, n = xforms.size(); i < n; ++i) {
		if ((xforms[i].m_componentFlags & flag) == flag) {
			math::Mat4f modelMx =
				math::Translate(xforms[i].position) *
				math::Rotation(xforms[i].rotation) *
				math::Scale(xforms[i].scale);
			m_shaderMap["DefaultShader"]->setUniform("model", modelMx);
			math::Vec3f abd = models[xforms[i].m_handlesToComponents[components::ComponentType<components::Model>::id]].albedo;
			m_shaderMap["DefaultShader"]->setUniform("albedo", models[xforms[i].m_handlesToComponents[components::ComponentType<components::Model>::id]].albedo);

			m_pUnitCube->render();
		}
	}

	math::Mat4f mvp = m_mainCam.viewPerspective()
		* math::Translate<F32>({0.f, -100.f, 0.f})
		* math::Scale<F32>({ 512.f, 1.f, 512.f });
	m_shaderMap["GridShader"]->use();

	m_gridTex.bind();
	m_shaderMap["GridShader"]->setUniform("tex", m_gridTex.glID());
	m_shaderMap["GridShader"]->setUniform("MVP", mvp);
	m_shaderMap["GridShader"]->setUniform("resolution", math::Vec2f{ 512.f, 512.f });
	m_shaderMap["GridShader"]->setUniform("tint", math::Vec4f{0.259f, 0.957f, 0.843f, 1.f });
	m_pGrid->render();

}

void RenderSystem::startFrame() {
	m_pRenderer->clear();

	// opaquePass();
	// transparentPass();
}

void RenderSystem::opaquePass() {
	// for each pass (color, depth ...)
	//	 for each shader
	//		for each material (textures, uniforms ...)
	//	

	// m_pRenderer->depthTest(true);
	// m_shaderMap["DefaultShader"]->use();

	for (auto pOpaque : m_opaqueArr)
		pOpaque->render();
}

void RenderSystem::transparentPass() {
	// sort back-to-front
	// for each pass (... blending ...)
	//	 for each shader
	//		for each material (...)

	for (auto pTransp : m_transpArr)
		pTransp->render();
}

void RenderSystem::endFrame() {
	/*m_pRenderer->bindWindowFrameBuffer();
	m_pRenderer->clear();

	m_pRenderer->depthTest(false);
	m_shaderMap["FrameDraw"]->use();*/

	// TODO (Simon): abstract the GL code shown below (E Z P Z)
	// glBindVertexArray(quadVAO);
	// glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
}


} // namespace gfx
} // namespace drak