#include "math.h"

namespace math
{
	float magnitude(const glm::vec3& v)
	{
		return sqrt(dot(v, v));
	}

	float distance(const glm::vec3& v1, const glm::vec3& v2)
	{
		return magnitude(v1 - v2);
	}

	float dot(const glm::vec3& v0, const glm::vec3& v1)
	{
		return (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
	}

	glm::vec3 cross(const glm::vec3& v0, const glm::vec3& v1)
	{
		return glm::vec3(v0.y * v1.z - v0.z * v1.y,
			v0.z * v1.x - v0.x * v1.z,
			v0.x * v1.y - v0.y * v1.x);
	}

	glm::vec3 normalize(const glm::vec3& v)
	{
		float m = magnitude(v);
		return glm::vec3(v.x / m, v.y / m, v.z /m);
	}

	glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
	{
		return v - n * dot(n, v) * 2.0f;
	}

	glm::vec3 calculate_normal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 normal = cross(edge1, edge2);
		return (normalize(normal));
	}

	glm::vec3 calculate_tangent(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec2 & uv0, const glm::vec2 & uv1, const glm::vec2 & uv2, const glm::vec3 & normal)
	{
		const int u = 0;
		const int v = 1;

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec2 edge1_uv = uv1 - uv0;
		glm::vec2 edge2_uv = uv2 - uv0;

		float f = 1.0f / ((edge1_uv[u] * edge2_uv[v]) - (edge1_uv[v] * edge2_uv[u]));

		glm::vec3 tangent;
		tangent.x = f * (edge2_uv[u] * edge1.x - edge1_uv[u] * edge2.x);
		tangent.y = f * (edge2_uv[u] * edge1.y - edge1_uv[u] * edge2.y);
		tangent.z = f * (edge2_uv[u] * edge1.z - edge1_uv[u] * edge2.z);
		tangent = glm::normalize(tangent);
		
		// Gram-Schmidt orthogonalize
		tangent = glm::normalize(tangent - (normal * glm::dot(normal, tangent)));

		return tangent;
	}

	void transform(std::vector<glm::vec3>& vectors, glm::mat4& matrix)
	{
		for (glm::vec3& v : vectors)
		{
			v = glm::vec4(v.x, v.y, v.z, 1.0f) * matrix;
		}
	}

	void transform(std::vector<glm::vec3>& vectors, glm::mat3& matrix)
	{
		for (glm::vec3& v : vectors)
		{
			v = v * matrix;
		}
	}
}
