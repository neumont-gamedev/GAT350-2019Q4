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
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		float u1_ = uv1[0] - uv0[0];
		float v1_ = uv1[1] - uv0[1];
		float u2_ = uv2[0] - uv0[0];
		float v2_ = uv2[1] - uv0[1];

		float r = 1.0f / (u1_ * v2_ - v1_ * u2_);
		glm::vec3 tangent = (edge1 * v2_ - edge2 * v1_) * r;

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
