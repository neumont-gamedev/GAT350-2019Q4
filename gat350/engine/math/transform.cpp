#include "transform.h"

void Transform::Edit()
{
	ImGui::Separator();
	ImGui::Text("Transform");
	//ImGui::TextColored((1, 0, 0, 1),"Transform");

	ImGui::SliderFloat3("Translation", (float*)&translation, -10.0f, 10.0f, "%.1f");
	ImGui::SliderFloat3("Scale", (float*)&scale, 0.0f, 10.0f);

	//glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
	//std::string rotation_string = "Rotation: " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z);
	//ImGui::Text(rotation_string.c_str());
	//mGui::gizmo3D("", rotation, 200.0f, imguiGizmo::mode3Axes | imguiGizmo::sphereAtOrigin);
	ImGui::gizmo3D("Rotation", rotation, 200.0f, imguiGizmo::mode3Axes | imguiGizmo::sphereAtOrigin);
}
