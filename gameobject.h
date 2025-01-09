#pragma once

#include "mathapi.h"
#include "model.h"
#include "IShader.h"

struct Transform {
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
};

class Object {
public:
	Transform transform;
	Object(Vector3f position = Vector3f::Zero, Vector3f rotation = Vector3f::Zero, Vector3f scale = Vector3f::One);
	virtual void tick(float delta_time);
	Matrix4x4 GetModelMatrix();
};

//¾²Ì¬Ä£ÐÍ
class Object_StaticModel : public Object
{
public:
	Model* StaticModel;
	ShaderInfo* StaticModelShaderInfo = new ShaderInfo;
	Object_StaticModel(Vector3f position = Vector3f::Zero, Vector3f rotation = Vector3f::Zero, Vector3f scale = Vector3f::One);
	Object_StaticModel(const char* filename, Vector3f position = Vector3f::Zero, Vector3f rotation = Vector3f::Zero, Vector3f scale = Vector3f::One);
	~Object_StaticModel();
};