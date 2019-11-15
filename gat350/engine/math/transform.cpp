#include "transform.h"

void Transform::Edit()
{
	ImGui::Text("Transform");
	
	ImGui::SliderFloat3("Translation", glm::value_ptr(translation), -20.0f, 20.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.0f, 10.0f);

	glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
	if (ImGui::SliderFloat3("Rotation", glm::value_ptr(euler), -360.0f, 360.0f))
	{
		rotation = glm::radians(euler);
	}
}
