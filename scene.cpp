#include "scene.h"

Scene::Scene(const char* name, Object_StaticModel *StaticModelObject) {
	this->name = name;
	Cameras = new Camera(CAMERA_POSITION, CAMERA_TARGET);
	Lights = new Object(Vector3f(2, 2, 2));
	StaticModels = StaticModelObject;
	StaticModels->StaticModelShaderInfo->des = "6.NormalMap";
	StaticModels->StaticModelShaderInfo->shader = new NormalMapShader;
	StaticModels->StaticModelShaderInfo->shader->shader_data = new ShaderData;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material = new Material;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->diffuse_map = StaticModelObject->StaticModel->get_diffuse_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->normal_map = StaticModelObject->StaticModel->get_normal_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->specular_map = StaticModelObject->StaticModel->get_specular_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->color = Color::White;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->specular = Color::White;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->gloss = 50;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->bump_scale = 1;
}

void Scene::AddStaticModel(Object_StaticModel *StaticModelObject)
{
	StaticModels = StaticModelObject;
	StaticModels->StaticModelShaderInfo->des = "6.NormalMap";
	StaticModels->StaticModelShaderInfo->shader = new NormalMapShader;
	StaticModels->StaticModelShaderInfo->shader->shader_data = new ShaderData;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material = new Material;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->diffuse_map = StaticModelObject->StaticModel->get_diffuse_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->normal_map = StaticModelObject->StaticModel->get_normal_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->specular_map = StaticModelObject->StaticModel->get_specular_map();
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->color = Color::White;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->specular = Color::White;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->gloss = 50;
	StaticModels->StaticModelShaderInfo->shader->shader_data->material->bump_scale = 1;
};