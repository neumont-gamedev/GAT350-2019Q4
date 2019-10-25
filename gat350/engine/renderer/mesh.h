#pragma once
#include "../engine.h"
#include "vertex_index_array.h"

#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}

	void Load(const std::string& filename);

	static bool Load(const std::string& filename, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	VertexIndexArray m_vertex_array;
	std::string m_directory;
};
