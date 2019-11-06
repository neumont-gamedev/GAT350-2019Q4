#include "model.h"
#include "mesh.h"

bool Model::Create(const Name& name)
{
	return Load(name.c_str());
}

void Model::Draw(GLenum primitiveType)
{
	for (Mesh& mesh : m_meshes)
	{
		mesh.material->Use();
		mesh.vertex_array->Draw(primitiveType);
	}
}

bool Model::Load(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
	{
		SDL_Log("Error opening file: %s", filename.c_str());
		return false;
	}

	// save directory for texture path
	m_directory = filename.substr(0, filename.find_last_of('/'));

	// get all nodes in scene
	std::vector<aiNode*> ai_nodes;
	ai_nodes.push_back(ai_scene->mRootNode);

	// collect scene nodes
	for (size_t i = 0; i < ai_nodes.size(); i++)
	{
		// get current node
		aiNode* ai_node = ai_nodes[i];
		// check for children in current node
		if (ai_node->mNumChildren == 0) continue;

		// add children of the current node
		for (size_t j = 0; j < ai_node->mNumChildren; j++)
		{
			ai_nodes.push_back(ai_node->mChildren[j]);
		}
	}

	for (size_t i = 0; i < ai_nodes.size(); i++)
	{
		aiNode* ai_node = ai_nodes[i];
		// check for meshes in current node
		if (ai_node->mNumMeshes == 0) continue;

		// process mesh and material
		for (size_t j = 0; j < ai_node->mNumMeshes; j++)
		{
			aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[j]];

			Mesh mesh;
			mesh.vertex_array = std::make_shared<VertexIndexArray>();
			ProcessMesh(ai_mesh, ai_scene, mesh.vertex_array);

			// check for material for this mesh
			aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
			if (ai_material)
			{
				mesh.material = std::make_shared<Material>();
				ProcessMaterial(ai_material, ai_scene, mesh.material);
			}

			m_meshes.push_back(mesh);
		}
	}

	return true;
}

void Model::ProcessMesh(aiMesh* ai_mesh, const aiScene* ai_scene, const std::shared_ptr<VertexIndexArray>& vertex_array)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> tangents;

	// get mesh vertex data
	for (size_t i = 0; i < ai_mesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = ai_mesh->mVertices[i].x;
		position.y = ai_mesh->mVertices[i].y;
		position.z = ai_mesh->mVertices[i].z;
		positions.push_back(position);

		if (ai_mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = ai_mesh->mNormals[i].x;
			normal.y = ai_mesh->mNormals[i].y;
			normal.z = ai_mesh->mNormals[i].z;
			normals.push_back(normal);
		}

		if (ai_mesh->HasTangentsAndBitangents())
		{
			glm::vec3 tangent;
			tangent.x = ai_mesh->mTangents[i].x;
			tangent.y = ai_mesh->mTangents[i].y;
			tangent.z = ai_mesh->mTangents[i].z;
			tangents.push_back(tangent);
		}

		if (ai_mesh->HasTextureCoords(0))
		{
			glm::vec2 texcoord;
			texcoord.x = ai_mesh->mTextureCoords[0][i].x;
			texcoord.y = ai_mesh->mTextureCoords[0][i].y;
			texcoords.push_back(texcoord);
		}
	}

	// create vertex array
	vertex_array->CreateBuffer(VertexArray::POSITION, static_cast<GLsizei>(positions.size() * sizeof(glm::vec3)), static_cast<GLsizei>(positions.size()), (void*)&positions[0]);
	vertex_array->SetAttribute(VertexArray::POSITION, 3, 0, 0);

	if (!normals.empty())
	{
		vertex_array->CreateBuffer(VertexArray::NORMAL, static_cast<GLsizei>(normals.size() * sizeof(glm::vec3)), static_cast<GLsizei>(normals.size()), (void*)&normals[0]);
		vertex_array->SetAttribute(VertexArray::NORMAL, 3, 0, 0);
	}

	if (!texcoords.empty())
	{
		vertex_array->CreateBuffer(VertexArray::TEXCOORD, static_cast<GLsizei>(texcoords.size() * sizeof(glm::vec2)), static_cast<GLsizei>(texcoords.size()), (void*)&texcoords[0]);
		vertex_array->SetAttribute(VertexArray::TEXCOORD, 2, 0, 0);
	}

	// create index buffer
	std::vector<GLuint> indices;
	for (size_t i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace* face = &ai_mesh->mFaces[i];
		for (size_t j = 0; j < face->mNumIndices; j++)
		{
			indices.push_back(face->mIndices[j]);
		}
	}
	vertex_array->CreateIndexBuffer(GL_UNSIGNED_INT, (GLsizei)indices.size(), &indices[0]);
}

void Model::ProcessMaterial(aiMaterial* ai_material, const aiScene* ai_scene, const std::shared_ptr<Material>& material)
{
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	if (ai_material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
	{
		material->ambient = glm::vec3(color.r, color.g, color.b);
	}

	if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		material->diffuse = glm::vec3(color.r, color.g, color.b);
	}

	if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		material->specular = glm::vec3(color.r, color.g, color.b);
	}

	if (ai_material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
	{
		material->shininess = shininess;
	}

	ProcessMaterialTextures(ai_material, aiTextureType_DIFFUSE, "texture_diffuse", material);
	ProcessMaterialTextures(ai_material, aiTextureType_NORMALS, "texture_normal", material);
}

void Model::ProcessMaterialTextures(aiMaterial* ai_material, aiTextureType type, std::string typeName, const std::shared_ptr<Material>& material)
{
	for (unsigned int i = 0; i < ai_material->GetTextureCount(type); i++)
	{
		aiString string;
		if (ai_material->GetTexture(type, i, &string) == AI_SUCCESS)
		{
			std::string filename = m_directory + '/' + string.C_Str();
			//SDL_Log("texture: %s", string.data);

			std::shared_ptr<Texture> texture = std::make_shared<Texture>();
			texture->Create(filename.c_str());
			material->textures.push_back(texture);
		}

		//// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		//bool skip = false;
		//for (unsigned int j = 0; j < textures_loaded.size(); j++)
		//{
		//	if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
		//	{
		//		textures.push_back(textures_loaded[j]);
		//		skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
		//		break;
		//	}
		//}
		//if (!skip)
		//{   // if texture hasn't been loaded already, load it
		//	Texture texture;
		//	texture.id = TextureFromFile(str.C_Str(), this->directory);
		//	texture.type = typeName;
		//	texture.path = str.C_Str();
		//	textures.push_back(texture);
		//	textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		//}
	}
}