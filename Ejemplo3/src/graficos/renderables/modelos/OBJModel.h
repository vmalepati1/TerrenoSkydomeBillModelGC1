#pragma once

#include "OBJLoader.h"
#include "graficos/renderables/Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "graficos/renderables/Renderable.h"
#include "OBJMesh.h"

class OBJModel : public Renderable {
private:
	objl::Loader *m_loader;
	std::vector<OBJMesh*> m_meshes;

public:
	OBJModel(const std::string &filepath, bool ccw, const mat4& transform, LightClass* lightSetup);
	~OBJModel();

	void Render(FPSCamara* camera, const vec4& clipPlane) override;
};