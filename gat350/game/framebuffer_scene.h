#pragma once
#include "../engine/framework/scene.h"

class FramebufferScene : public Scene
{
public:
	OBJECT_DECLARATION(FramebufferScene, Scene)

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;

protected:
	void RenderToTexture();
	void RenderScene();
};