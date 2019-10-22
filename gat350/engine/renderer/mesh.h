#pragma once
#include "../engine.h"

class Mesh
{
public:
	static bool Load(const std::string& filename, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords);
	static void CalculateNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& normal);
};
