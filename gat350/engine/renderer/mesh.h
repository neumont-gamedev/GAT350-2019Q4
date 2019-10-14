#pragma once
#include "../engine.h"
#include "vertex_index_array.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Import(const std::string& filename);
	void Draw();

	//static bool Load(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& uvs);

private:
	//void ProcessNode(aiNode* node, const aiScene* scene, const glm::vec3& translate, const glm::vec3& scale);
	//void ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::vec3& translate, const glm::vec3& scale);

protected:
	//VertexIndexArray m_vertex_array;
};
