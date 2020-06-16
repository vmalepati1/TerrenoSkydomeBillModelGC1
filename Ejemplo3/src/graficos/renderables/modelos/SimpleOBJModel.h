#pragma once

#include "graficos/renderables/Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "graficos/renderables/Renderable.h"
#include "OBJLoader.h"

class SimpleOBJModel : public Renderable {
public:
	SimpleOBJModel(const std::string &filepath, const wchar_t* textura, bool ccw, const mat4& transform, LightClass* lightSetup);
	~SimpleOBJModel();

	void Render(FPSCamara* camera, const vec4& clipPlane) override;
private:
	objl::Loader* m_loader;
	unsigned int m_textureID;
	bool m_ccw;

	void PushBuffers() override;
	void BindBuffers() override;
	void UnbindBuffers() override;
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;
};