#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

#include <iostream>
#include <typeinfo>

#include "SRGraphics.h"
#include "Render.h"
#include "Window.h"

#include "ResourceManager.h"

using namespace SpaRcle::Graphics;

SpaRcle::Graphics::GameObject::GameObject(std::string name, const bool editor_object) : m_editor_object(editor_object) {
    this->m_name        = name;
    this->m_transform   = new Transform(this);
}

SpaRcle::Graphics::GameObject::~GameObject() {
    for (auto a : this->m_components)
        if (a->TypeName() == "Mesh") {
            SRGraphics::Get()->GetMainWindow()->GetRender()->RemoveMesh(static_cast<Mesh*>(a));
        }
    delete m_transform;
    m_transform = nullptr;
}

void SpaRcle::Graphics::GameObject::InvertSelect() {
    this->m_is_select = !m_is_select;
    if (m_is_select)
        ResourceManager::AddSelectedGameObject(this);
    else
        ResourceManager::RemoveSelectedGameObject(this);
}

bool SpaRcle::Graphics::GameObject::AddComponent(Component* component) {
    //std::cout << component->TypeName() << std::endl;

    if (component->TypeName() == "Mesh")
        SRGraphics::Get()->GetMainWindow()->GetRender()->AddMesh(static_cast<Mesh*>(component));

    this->m_components.push_back(component);
    return true;
}

//!=========================================================================================================

std::vector<SpaRcle::Graphics::GameObject*> SpaRcle::Graphics::GameObject::Find(std::string name) {
    Debug::Error("GameObject::Find() : failed find \"" + name + "\" game object!");
    return {};
}

std::vector<GameObject*> SpaRcle::Graphics::GameObject::FindOfTag(std::string tag)
{
    Debug::Error("GameObject::FindOfTag() : failed find \"" + tag + "\" game object!");
    return {};
}

bool SpaRcle::Graphics::GameObject::Destroy(std::string name) {
    Debug::Error("GameObject::Destroy() : failed destroyind \"" + name + "\" game object! (Not found)");
    return false;
}

bool SpaRcle::Graphics::GameObject::Destroy(GameObject* gameObject) {
    if (!gameObject) {
        Debug::Error("GameObject::Destroy() : failed destroying !nullptr! game object!");
        return false;
    }
    else {
        Debug::Log("GameObject::Destroy() : destroying "+gameObject->m_name + " game object...");

        bool d = false;
        for (size_t t = 0; t < ResourceManager::m_gameObjects.size(); t++) {
            if (ResourceManager::m_gameObjects[t] == gameObject) {
                ResourceManager::m_gameObjects.erase(ResourceManager::m_gameObjects.begin() + t);
                Sleep(5);
                delete gameObject;
                d = true;
                break;
            }
        }

        if (d)
            return true;
        else {
            Debug::Error("GameObject::Destroy() : failed destroyind \"" + gameObject->m_name + "\" game object! (Not found)");
            return false;
        }
    }
}

SpaRcle::Graphics::GameObject* SpaRcle::Graphics::GameObject::Instance(std::string name, const bool editor) {
    Debug::Log("GameObject::Instance() : instance new \"" + name + "\" game object...");

    GameObject* gm = new GameObject(name, editor);

    ResourceManager::m_gameObjects.push_back(gm);

    return gm;
}
