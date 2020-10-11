#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

SpaRcle::Graphics::GameObject::GameObject(std::string name) {
    this->m_name        = name;
    this->m_transform   = new Transform(this);
}

SpaRcle::Graphics::GameObject::~GameObject() {
    delete m_transform;
    m_transform = nullptr;
}

bool SpaRcle::Graphics::GameObject::AddComponent(Component* component) {
    this->m_components.push_back(component);
    return true;
}

bool SpaRcle::Graphics::GameObject::Destroy(GameObject* gameObject)
{
    return false;
}

SpaRcle::Graphics::GameObject* SpaRcle::Graphics::GameObject::Instance(std::string name) {
    GameObject* gm = new GameObject(name);
    return gm;
}
