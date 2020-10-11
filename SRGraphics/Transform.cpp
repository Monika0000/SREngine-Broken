#include "pch.h"
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject) {
	this->m_gm_components = &gameObject->m_components;
}

SpaRcle::Graphics::Transform::Transform(GameObject* gameObject, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)  {
	this->m_gm_components = &gameObject->m_components;
}

void SpaRcle::Graphics::Transform::Translate(glm::vec3 translation, bool local) {
	if (!local) {
		this->m_position += translation;
	}
	else {
		this->m_position += translation;
	}

	for (auto a : *m_gm_components)
		a->OnMoved(this->m_position);
}

void SpaRcle::Graphics::Transform::Rotate(glm::vec3 eulerAngles, bool local) { }

void SpaRcle::Graphics::Transform::Scaling(glm::vec3 scale, bool local) { }

void SpaRcle::Graphics::Transform::Translate(float x, float y, float z, bool local) { this->Translate({ x,y,z },	local);		}
void SpaRcle::Graphics::Transform::Rotate(float x, float y, float z,	bool local)	{ this->Rotate({x,y,z},			local);		}
void SpaRcle::Graphics::Transform::Scaling(float x, float y, float z,	bool local)	{ this->Scaling({ x,y,z },		local);		}
