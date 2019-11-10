#pragma once
#include "../engine/framework/scene.h"

class GameScene : public Scene
{
public:
	OBJECT_DECLARATION(GameScene, Scene)

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;
};