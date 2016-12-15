#include "Skybox.h"

//Constructor(s) & Destructor
Skybox::Skybox() : mesh(MeshBuilder::GetInstance().GetMesh("Skybox")) {
}

Skybox::~Skybox() {
}

//Function(s)
Mesh* Skybox::GetMesh() const {
	return this->mesh;
}