#pragma once
#include "../framework/resource.h"
#include "vertex_index_array.h"
#include "material.h"

#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Resource
{
public:
	struct Mesh
	{
		std::shared_ptr<VertexIndexArray> vertex_array;
		std::shared_ptr<Material> material;
	};

public:
	OBJECT_DECLARATION(Model, Resource)
	~Model() {}

	bool Create(const Name& name);
	void Destroy() {}

	void Draw(GLenum primitiveType = GL_TRIANGLES);
	bool Load(const std::string& filename);

private:
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<VertexIndexArray>& vertex_array);
	void ProcessMaterial(aiMaterial* ai_material, const aiScene* scene, const std::shared_ptr<Material>& material);
	void ProcessMaterialTextures(aiMaterial* ai_material, aiTextureType type, std::string typeName, const std::shared_ptr<Material>& material);

private:
	std::vector<Mesh> m_meshes;
	std::string m_directory;
};
