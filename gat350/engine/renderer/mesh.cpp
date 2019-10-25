#include "mesh.h"

void Mesh::Load(const std::string& filename)
{
	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//// check for errors
	//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	//{
	//	return;
	//}
	//// retrieve the directory path of the filepath
	//m_directory = filename.substr(0, filename.find_last_of('/'));

	//// process ASSIMP's root node recursively
	//ProcessNode(scene->mRootNode, scene);

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
	std::vector<glm::vec3> tangents;

	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		positions.push_back(position);

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		normals.push_back(normal);

		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		tangents.push_back(tangent);

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texcoord;
			texcoord.x = mesh->mTextureCoords[0][i].x;
			texcoord.y = mesh->mTextureCoords[0][i].y;
			texcoords.push_back(texcoord);
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			tangents.push_back(tangent);
		}
	}

	//m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, sizeof(glm::vec3), (GLsizei)positions.size(), &positions[0]);
	//m_vertexArrays.SetAttribute(0, 3, sizeof(glm::vec3), 0);
	//if (normals.size() > 0)
	//{
	//	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, sizeof(glm::vec3), (GLsizei)normals.size(), &normals[0]);
	//	m_vertexArrays.SetAttribute(1, 3, sizeof(glm::vec3), 0);
	//}
	//if (texcoords.size() > 0)
	//{
	//	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, sizeof(glm::vec2), (GLsizei)texcoords.size(), &texcoords[0]);
	//	m_vertexArrays.SetAttribute(2, 2, sizeof(glm::vec2), 0);
	//}
	//if (tangents.size() > 0)
	//{
	//	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TANGENT, sizeof(glm::vec3), (GLsizei)tangents.size(), &tangents[0]);
	//	m_vertexArrays.SetAttribute(3, 3, sizeof(glm::vec3), 0);
	//}

	//std::vector<GLuint> indices;
	//for (size_t i = 0; i < mesh->mNumFaces; i++)
	//{
	//	aiFace face = mesh->mFaces[i];
	//	for (size_t j = 0; j < face.mNumIndices; j++)
	//	{
	//		indices.push_back(face.mIndices[j]);
	//	}
	//}

	//m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_INT, (GLsizei)indices.size(), &indices[0]);
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

