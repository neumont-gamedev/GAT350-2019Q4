#include "light_scene.h"
#include "lights_scene.h"
#include "normal_map_scene.h"
#include "fx_scene.h"
#include "../engine/engine.h"
#include "../engine/editor/editor.h"

int main(int argc, char** argv)
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	engine->Initialize();

	//std::unique_ptr<Scene> scene = std::make_unique<FXScene>(FXScene::GetClassName(), engine.get());
	//std::unique_ptr<Scene> scene = std::make_unique<LightScene>(LightScene::GetClassName(), engine.get());
	std::unique_ptr<Scene> scene = std::make_unique<NormalMapScene>(NormalMapScene::GetClassName(), engine.get());
	scene->Create("scene");

	engine->Get<Editor>()->m_scene = scene.get();

	while (!engine->IsQuit())
	{
		engine->Update();

		scene->Update();
		scene->Draw();
	}
	engine->Shutdown();
	
	return 0;
}
