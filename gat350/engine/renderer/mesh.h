#pragma once
#include "../engine.h"
#include "vertex_index_array.h"
#include "material.h"

#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	static bool Load(const std::string& filename, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords);
};
