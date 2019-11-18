#include "transform.h"

void Transform::Edit()
{
	ImGui::Separator();
	ImGui::Text("Transform");
	
	ImGui::SliderFloat3("Translation", glm::value_ptr(translation), -20.0f, 20.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.0f, 10.0f);

	glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
	std::string rotation_string = "Rotation: " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z);

	ImGui::gizmo3D(rotation_string.c_str(), rotation, 200.0f, imguiGizmo::mode3Axes | imguiGizmo::sphereAtOrigin);
}
