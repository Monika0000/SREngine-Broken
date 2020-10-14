#include "pch.h"
#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject) {
	this->m_gm_components = &gameObject->m_components;
}

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)  {
	this->m_gm_components = &gameObject->m_components;
}

void SpaRcle::Graphics::Transform::SetPosition(glm::vec3 pos)
{
}

void SpaRcle::Graphics::Transform::SetRotation(glm::vec3 rot) {
	this->m_rotation = rot;
	//this->m_rotation = {
		//rot.x - int(int(rot.x) / 360) * 360,
		//rot.y - int(int(rot.y) / 360) * 360,
		//rot.z - int(int(rot.z) / 360) * 360
	//};
	for (auto a : *m_gm_components)
		a->OnRotated(this->m_rotation);
}

void SpaRcle::Graphics::Transform::SetScale(glm::vec3 scl)
{
}

void SpaRcle::Graphics::Transform::SetPosition(float x, float y, float z)	{ this->SetPosition({x,y,z});	}
void SpaRcle::Graphics::Transform::SetRotation(float x, float y, float z)	{ this->SetRotation({ x,y,z });	}
void SpaRcle::Graphics::Transform::SetScale(float x, float y, float z)		{ this->SetScale({ x,y,z });	}

void SpaRcle::Graphics::Transform::Translate(glm::vec3 translation, bool local) {
	if (!local) {
		this->m_position += translation;
	}
	else {
		float dz = cos(this->m_rotation.y * 3.14 / 45.f / 4.f);
		float dx = sin(this->m_rotation.y * 3.14 / 45.f / 4.f);

		/*this->m_position += glm::vec3(
			0 + translation.z * -dx,
			translation.y,
			translation.z * dz + 0
		);*/

		/*this->m_position += glm::vec3(
			translation.x * dz + 0,
			translation.y,
			0 + translation.x * dx
		);*/

		this->m_position += glm::vec3(
			translation.x * dz - translation.z * dx,
			translation.y,
			translation.z * dz + translation.x * dx
		);
	}

	for (auto a : *m_gm_components)
		a->OnMoved(this->m_position);
}

void SpaRcle::Graphics::Transform::Rotate(glm::vec3 eulerAngles, bool local) { }

void SpaRcle::Graphics::Transform::Scaling(glm::vec3 scale, bool local) { }

void SpaRcle::Graphics::Transform::Translate(float x, float y, float z, bool local) { this->Translate({ x,y,z },	local);		}
void SpaRcle::Graphics::Transform::Rotate(float x, float y, float z,	bool local)	{ this->Rotate({x,y,z},			local);		}
void SpaRcle::Graphics::Transform::Scaling(float x, float y, float z,	bool local)	{ this->Scaling({ x,y,z },		local);		}
