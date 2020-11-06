#include "pch.h"
#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject) {
	this->m_gm_components = &gameObject->m_components;
	this->m_gameObject = gameObject;
}

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)  {
	this->m_gm_components = &gameObject->m_components;
	this->m_gameObject = gameObject;
}

void SpaRcle::Graphics::Transform::UpdateChild(Transform* parent) {
	this->m_parent_position = parent->m_position + parent->m_parent_position;
	this->m_parent_rotation = parent->m_rotation + parent->m_parent_rotation;
	this->m_parent_scale	= parent->m_scale    + parent->m_parent_scale;

	Translate(0, 0, 0);
	Rotate(0, 0, 0);
	Scaling(0, 0, 0);
}

void SpaRcle::Graphics::Transform::SetPosition(glm::vec3 pos) {
	this->m_position = pos;

	for (auto a : *m_gm_components)
		a->OnMoved(this->m_position + m_parent_position);

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->SetPosition(pos);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}
void SpaRcle::Graphics::Transform::SetRotation(glm::vec3 rot) {
	this->m_rotation = rot;

	for (auto a : *m_gm_components)
		a->OnRotated(this->m_rotation + m_parent_rotation);

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->SetRotation(rot);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}
void SpaRcle::Graphics::Transform::SetScale(glm::vec3 scale) {
	this->m_scale = scale;
	for (auto a : *m_gm_components)
		a->OnScaled(m_gameObject->m_parent ? this->m_scale * m_parent_scale : m_scale);

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->SetScale(scale);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}

void SpaRcle::Graphics::Transform::SetPosition(float x, float y, float z)	{ this->SetPosition({x,y,z});	}
void SpaRcle::Graphics::Transform::SetRotation(float x, float y, float z)	{ this->SetRotation({ x,y,z });	}
void SpaRcle::Graphics::Transform::SetScale(float x, float y, float z)		{ this->SetScale({ x,y,z });	}

glm::vec3 SpaRcle::Graphics::Transform::Forward() { return LocalDirection(m_forward); }
glm::vec3 SpaRcle::Graphics::Transform::Right() { return LocalDirection(m_right); }
glm::vec3 SpaRcle::Graphics::Transform::Up() { return LocalDirection(m_up); }

glm::vec3 SpaRcle::Graphics::Transform::LocalDirection(const glm::vec3& dir) {
	const float dyz = cos(this->m_rotation.y * 3.14 / 45.f / 4.f);
	const float dyx = sin(this->m_rotation.y * 3.14 / 45.f / 4.f);

	const float dzx = cos(this->m_rotation.z * 3.14 / 45.f / 4.f);
	const float dzy = sin(this->m_rotation.z * 3.14 / 45.f / 4.f);

	//return glm::vec3(
	//	dir.x * dyz * dzx - dir.z * dyx * dzx,
	//	dir.y - dir.x * dzy,
	//	0
	//);
	return glm::vec3(
		dir.x * dyz - dir.z * dyx,
		dir.y,
		dir.x * dyx + dir.z * dyz
	);
}

void SpaRcle::Graphics::Transform::Translate(glm::vec3 translation, bool local) {
	//float dz = cos(this->m_rotation.y * 3.14 / 45.f / 4.f);
	//float dx = sin(this->m_rotation.y * 3.14 / 45.f / 4.f);

	if (!local) {
		m_position += translation;
	}
	else {
		//m_position += glm::vec3(
		//	translation.x * dz + translation.z * dx,
		//	translation.y,
		//	translation.z * dz - translation.x * dx
		//);
		this->m_position += LocalDirection(translation);

		/*
			translation.x * dz + translation.z * dx,
			translation.y,
			translation.z * dz - translation.x * dx
		*/
	}

	for (auto a : *m_gm_components) {
		a->OnMoved(this->m_position + m_parent_position);
	}

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->Translate(translation);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}
void SpaRcle::Graphics::Transform::Rotate(glm::vec3 eulerAngles, bool local) {
	this->m_rotation += eulerAngles;
	for (auto a : *m_gm_components)
		a->OnRotated(this->m_rotation + m_parent_rotation);

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->Rotate(eulerAngles);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}

void SpaRcle::Graphics::Transform::Scaling(glm::vec3 scale, bool local, bool mul) { 
	if(mul)
		this->m_scale *= scale;
	else
		this->m_scale += scale;

	for (auto a : *m_gm_components)
		a->OnScaled(m_gameObject->m_parent ? this->m_scale * m_parent_scale : m_scale);
	
	//a->OnScaled(this->m_scale * m_parent_scale);

	//for (auto a : this->m_gameObject->m_childrens) {
	//	a->m_transform->Scaling(scale, mul);
	//}

	for (auto a : this->m_gameObject->m_childrens)
		a->m_transform->UpdateChild(this->m_gameObject->m_transform);
}

void SpaRcle::Graphics::Transform::Translate(float x, float y, float z, bool local) { this->Translate({ x,y,z },	local);		}
void SpaRcle::Graphics::Transform::Rotate(float x, float y, float z,	bool local)	{ this->Rotate({x,y,z},			local);		}
void SpaRcle::Graphics::Transform::Scaling(float x, float y, float z,	bool local)	{ this->Scaling({ x,y,z },		local);		}
