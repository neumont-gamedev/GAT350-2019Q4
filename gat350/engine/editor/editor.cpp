#include "editor.h"
#include "../engine.h"
#include "../input/input.h"

bool Editor::Initialize()
{
	m_engine->Get<Input>()->AddAction("visible", SDL_SCANCODE_GRAVE, Input::KEYBOARD);

	return true;
}

void Editor::Shutdown()
{
}

void Editor::Update()
{
	if (m_engine->Get<Input>()->GetButton("visible", Input::eButtonState::PRESSED)) m_visible = !m_visible;
}

void Editor::UpdateGUI()
{
	if (m_visible == false) return;

	if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen) == false) return;
	std::vector<Actor*> actors = m_scene->Get<Actor>();
	for (Actor* actor : actors)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (m_actor == actor) node_flags |= ImGuiTreeNodeFlags_Selected;
		
		ImColor color = (actor->m_active) ? ImColor(1.0f, 1.0f, 1.0f) : ImColor(0.5f, 0.5f, 0.5f);
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)color);
		ImGui::TreeNodeEx(actor, node_flags, actor->m_name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_actor = actor;
		}
		ImGui::PopStyleColor();
	}

	if (ImGui::CollapsingHeader("Actor", ImGuiTreeNodeFlags_DefaultOpen) == false) return;
	if (m_actor)
	{
		m_actor->Edit();
	}


}
