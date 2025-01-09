#pragma once

#include "scene.h"

class Renderer {
public:
	RenderBuffer* frameBuffer = nullptr;
	RenderBuffer* ShadowMap = nullptr;
	bool enable_shadow = true;
	Scene* RenderScene = nullptr;
	ShaderInfo ShaderInfo[6];
	int ShaderIndex = 0;
	ShaderData* shader_data = new ShaderData;
	DrawData* ShadowDrawData = nullptr;
	Camera* Camera = nullptr;
	char text[500];

	Renderer(RenderBuffer* frameBuffer,Scene* RenderScene);

	virtual void Render();
	virtual void RenderShadow();
	virtual void ClearShadowMap();
	virtual void on_key_input(keycode_t key, int pressed);
	virtual void InitializeShaderInfo();
	void PrintSceneInfo();
};