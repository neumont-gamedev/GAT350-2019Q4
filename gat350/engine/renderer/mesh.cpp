#include "mesh.h"

void Mesh::Draw(GLenum primitiveType)
{
	m_material.Use();
	m_vertex_array.Draw(primitiveType);
}

bool Mesh::Load(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SDL_Log("Error opening file: %s", filename.c_str());
		return false;
	}

	m_directory = filename.substr(0, filename.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> tangents;
	
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		positions.push_back(position);

		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			normals.push_back(normal);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			glm::vec3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			tangents.push_back(tangent);
		}

		if (mesh->HasTextureCoords(0))
		{
			glm::vec2 texcoord;
			texcoord.x = mesh->mTextureCoords[0][i].x;
			texcoord.y = mesh->mTextureCoords[0][i].y;
			texcoords.push_back(texcoord);
		}
	}

	m_vertex_array.CreateBuffer(VertexArray::POSITION, static_cast<GLsizei>(positions.size() * sizeof(glm::vec3)), static_cast<GLsizei>(positions.size()), (void*)&positions[0]);
	m_vertex_array.SetAttribute(VertexArray::POSITION, 3, 0, 0);

	if (!normals.empty())
	{
		m_vertex_array.CreateBuffer(VertexArray::NORMAL, static_cast<GLsizei>(normals.size() * sizeof(glm::vec3)), static_cast<GLsizei>(normals.size()), (void*)&normals[0]);
		m_vertex_array.SetAttribute(VertexArray::NORMAL, 3, 0, 0);
	}

	if (!texcoords.empty())
	{
		m_vertex_array.CreateBuffer(VertexArray::TEXCOORD, static_cast<GLsizei>(texcoords.size() * sizeof(glm::vec2)), static_cast<GLsizei>(texcoords.size()), (void*)&texcoords[0]);
		m_vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 0, 0);
	}

	// index buffer
	std::vector<GLuint> indices;
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace* face = &mesh->mFaces[i];
		for (size_t j = 0; j < face->mNumIndices; j++)
		{
			indices.push_back(face->mIndices[j]);
		}
	}
	m_vertex_array.CreateIndexBuffer(GL_UNSIGNED_INT, (GLsizei)indices.size(), &indices[0]);

	// materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	if (material)
	{
		ProcessMaterial(material, scene);
	}
}

void Mesh::ProcessMaterial(aiMaterial* material, const aiScene* scene)
{
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
	{
		m_material.ambient = glm::vec3(color.r, color.g, color.b);
	}

	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		m_material.diffuse = glm::vec3(color.r, color.g, color.b);
	}

	if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		m_material.specular = glm::vec3(color.r, color.g, color.b);
	}
		
	if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
	{
		m_material.shininess = shininess;
	}

	std::vector<Texture*> diffuse_textures = ProcessMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	m_material.textures.insert(m_material.textures.end(), diffuse_textures.begin(), diffuse_textures.end());

	std::vector<Texture*> normal_textures = ProcessMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	m_material.textures.insert(m_material.textures.end(), normal_textures.begin(), normal_textures.end());
}

std::vector<Texture*> Mesh::ProcessMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString string;
		if (material->GetTexture(type, i, &string) == AI_SUCCESS)
		{
			std::string filename = m_directory + '/' + string.C_Str();
			SDL_Log("texture: %s", filename.c_str());
			//Texture* texture = new Texture;
			//texture->CreateTexture(filename);
			//textures.push_back(texture);
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
	return textures;
}

bool Mesh::Load(const std::string& filename, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords)
{
	std::vector<glm::vec3> mesh_positions;
	std::vector<glm::vec3> mesh_normals;
	std::vector<glm::vec2> mesh_texcoords;

#if 1
	std::ifstream stream(filename, std::ios::binary);
	if (!stream.is_open())
	{
		SDL_Log("Error opening file: %s", filename.c_str());
		return false;
	}
#else
	std::string stream_string;
	filesystem::read_file(filename.c_str(), stream_string);
	std::istringstream stream(stream_string);
#endif

	std::string line;
	while (std::getline(stream, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream string_stream(line.substr(2));
			glm::vec3 position;
			string_stream >> position.x;
			string_stream >> position.y;
			string_stream >> position.z;
			mesh_positions.push_back(position);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream string_stream(line.substr(3));
			glm::vec3 normal;
			string_stream >> normal.x;
			string_stream >> normal.y;
			string_stream >> normal.z;
			mesh_normals.push_back(normal);
		}
		else if (line.substr(0, 3) == "vt ")
		{
			std::istringstream string_stream(line.substr(3));
			glm::vec2 texcoord;
			string_stream >> texcoord.x;
			string_stream >> texcoord.y;
			mesh_texcoords.push_back(texcoord);
		}
		if (line.substr(0, 2) == "f ")
		{
			std::istringstream string_stream(line.substr(2));
			std::string vertex_string;
			while (std::getline(string_stream, vertex_string, ' '))
			{
				std::istringstream vertex_stream(vertex_string);
				std::string index_string;

				size_t i = 0;
				u32 index[3] = { 0, 0, 0 };
				while (std::getline(vertex_stream, index_string, '/'))
				{
					if (!index_string.empty())
					{
						std::istringstream index_stream(index_string);
						index_stream >> index[i];
					}
					i++;
				}

				if (index[0])
				{
					glm::vec3 position = mesh_positions[index[0] - 1];
					positions.push_back(position);
				}

				if (index[1])
				{
					glm::vec2 texcoord = mesh_texcoords[index[1] - 1];
					texcoords.push_back(texcoord);
				}

				if (index[2])
				{
					glm::vec3 normal = mesh_normals[index[2] - 1];
					normals.push_back(normal);
				}
			}
		}
	}
	stream.close();

	return true;
}

