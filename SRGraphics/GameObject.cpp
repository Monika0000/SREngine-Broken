#include "pch.h"
#include "GameObject.h"
#include "Component.h"

bool SpaRcle::Graphics::GameObject::AddComponent(Component* component) {

    return true;
}

bool SpaRcle::Graphics::GameObject::Destroy(GameObject* gameObject)
{
    return false;
}

SpaRcle::Graphics::GameObject* SpaRcle::Graphics::GameObject::Instance(std::string name)
{
    return nullptr;
}
