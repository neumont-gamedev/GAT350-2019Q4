#pragma once
#include "../engine.h"

class Model
{
public:
	Model() {}
	~Model() {}

	void Load(const std::string& filename);

private:
	//void ProcessNode(aiNode* node, const aiScene* scene);
	//void ProcessMesh(aiMesh* mesh, const aiScene* scene);


private:
	std::vector<class Mesh*> m_meshes;
	std::string m_directory;
};
