#include "Mesh.h"

Mesh* Mesh::quadMesh = nullptr;

Mesh::Mesh(boost::filesystem::path path) {
	this->model = new AssetModel(path);
	this->vertices = this->model->meshes[0].vertices;
	this->indices = this->model->meshes[0].indices;
}