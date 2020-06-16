#include "OBJModel.h"

OBJModel::OBJModel(const std::string& filepath, bool ccw, const mat4& transform, LightClass* lightSetup) 
	: Renderable(transform, lightSetup) {

	m_loader = new objl::Loader();
	m_loader->LoadFile(filepath);

	for (const objl::Mesh& m : m_loader->LoadedMeshes) {
		m_meshes.push_back(new OBJMesh(m, ccw, transform, lightSetup));
	}
}

OBJModel::~OBJModel() {
	delete m_loader;

	for (OBJMesh* m : m_meshes) {
		delete m;
	}
}

void OBJModel::Render(FPSCamara* camera, const vec4& clipPlane) {
	for (OBJMesh *m : m_meshes) {
		m->Render(camera, clipPlane);
	}
}