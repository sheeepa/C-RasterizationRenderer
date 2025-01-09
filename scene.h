#pragma once

#include "mathapi.h"
#include "gameobject.h"
#include "camera.h"
#include "platform.h"

static Color AMBIENT = Color(54.f / 255, 58.f / 255, 66.f / 255);
static Color LightColor = Color(255.f / 255, 244.f / 255, 214.f / 255);

struct Scene {
public:
	const char* name;
	Object* Lights;
	Camera* Cameras;
	Object_StaticModel* StaticModels;

	Scene(const char* name, Object_StaticModel* StaticModelObject);

	void LoadRenderModel(int scene_index);
	
	void AddStaticModel(Object_StaticModel* StaticModelObject);
};