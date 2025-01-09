#include "gameobject.h"

Object::Object(Vector3f position, Vector3f rotation, Vector3f scale) {
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;
}

void Object::tick(float delta_time) {
}

Matrix4x4 Object::GetModelMatrix() {
	Matrix4x4 m_translation = translate(transform.position.x, transform.position.y, transform.position.z);
	Matrix4x4 m_rotate = rotate_z(TO_RADIANS(transform.rotation.z)) * rotate_x(TO_RADIANS(transform.rotation.x)) * rotate_y(TO_RADIANS(transform.rotation.y));
	Matrix4x4 m_scale = scale(transform.scale.x, transform.scale.y, transform.scale.z);
	return m_translation * m_rotate * m_scale;
}

Object_StaticModel::Object_StaticModel(Vector3f position, Vector3f rotation, Vector3f scale) :Object(position, rotation, scale) {}

Object_StaticModel::Object_StaticModel(const char* filename, Vector3f position, Vector3f rotation, Vector3f scale) :Object(position, rotation, scale) {
	this->StaticModel = new Model(filename);
}

Object_StaticModel::~Object_StaticModel() {
	delete StaticModel;
}