#pragma once
#include "../def.h"

class Transform
{
public:
	Transform() : translation(0.0f), rotation(glm::vec3(0.0f)), scale(1.0f) {}
	Transform(const glm::vec3& translation, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
		: translation(translation), rotation(rotation), scale(scale) {}
	Transform(const glm::mat4& matrix) { *this = matrix; }

	inline glm::mat4 GetMatrix() const
	{
		return *this;
	}

	inline operator glm::mat4 () const
	{
		// create matrix from translation, rotation and scale
		glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), translation);

		return (translation_matrix * rotation_matrix * scale_matrix);
	}

	inline Transform& operator = (const glm::mat4& matrix)
	{
		// decompose matrix to translation, rotation and scale
		translation = glm::vec3(matrix[3]);
		rotation = glm::quat_cast(matrix);
		scale = glm::vec3(matrix[0][0], matrix[1][1], matrix[2][2]);

		return *this;
	}

	inline void Rotate(const glm::quat& rotate)
	{
		rotation = rotation * rotate;
	}

	void Edit();

public:
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
};
