#pragma once
#include "../def.h"
#include "vertex_index_array.h"
#include "material.h"

class Mesh
{
public:
	static bool Load(const std::string& filename, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords);
};
