#include "math.h"

namespace Math
{
	float length(const glm::vec3& v)
	{
		return sqrt(dot(v, v));
	}

	float distance(const glm::vec3& v1, const glm::vec3& v2)
	{
		return length(v1 - v2);
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
		return v / length(v);
	}

	glm::vec3 calculate_normal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec3 v1_0 = v1 - v0;
		glm::vec3 v2_0 = v2 - v0;

		glm::vec3 normal = cross(v1 - v0, v2 - v0);
		return (normalize(normal));
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
