﻿#include "pch.h"
#include "SREngine.h"
#include <Input.h>
#include <EventsManager.h>
#include <ResourceManager.h>

using namespace SpaRcle::Helper;
using namespace SpaRcle::Graphics;

bool SpaRcle::Engine::SREngine::Create(Window* win) {
    if (m_isCreated) {
        Debug::Error("Game engine already created!");
        return false;
    } else Debug::Info("Creating game engine...");

    this->m_resource_folder = ResourceManager::GetResourceFolder();

    {
        this->m_compiler    = new Compiler();
        this->m_graph       = SRGraphics::Get();

        this->m_graph->Create(win, m_resource_folder);
    }

    this->m_camera = m_graph->GetMainWindow()->GetCameraGameObject();

    this->m_isCreated = true;
    return true;
}

bool SpaRcle::Engine::SREngine::Init() {
    if (!m_isCreated) {
        Debug::Error("Engine must be created before initialization!");
        return false;
    }

    if (m_isInit) {
        Debug::Error("Game engine already initialize!");
        return false;
    } else Debug::Info("Initializing game engine...");

    if (!this->m_graph->Init()) {
        Debug::Error("Failed initializing graphics engine!");
        return false;
    }

    this->m_isInit = true;
    return true;
}


bool SpaRcle::Engine::SREngine::Run() {
    if (!m_isInit) {
        Debug::Error("Engine must be initialized before running!");
        return false;
    }

    if (m_isRunning) {
        Debug::Error("Game engine already running!");
        return false;
    } else Debug::Info("Running game engine...");

    if (!this->m_graph->Run()) {
        Debug::Error("Failed running graphics engine!");
        return false;
    }

    this->m_graph->GetMainWindow()->WaitWindowRunning();

    this->m_isRunning = true;
    
    Debug::System("All systems ran successfully!");

    //!=================================================================================
    {
        Script* scene_manager = new Script("scene_manager");
        this->m_compiler->AddScript(scene_manager);

        std::vector<Mesh*> meshes = ResourceManager::LoadObjModel("Sina.obj");
        this->m_graph->GetMainWindow()->GetRender()->AddMeshes(meshes);
    }
    //!=================================================================================

    bool break_event = false;
    bool is_awake_scripts = false;

    float camera_speed = 0.0001;

    while (this->m_isRunning) {
        switch (EventsManager::PopEvent()) {
        case SpaRcle::Helper::EventsManager::Event::Exit: 
            Debug::Info("SREngine::Run() : exit from game engine...");
            break_event = true;
            break;
        case SpaRcle::Helper::EventsManager::Event::Error: 
            Debug::Info("SREngine::Run() : An exception has been occured!");
            break_event = true;
            break;
        }
        if (break_event) break;

        if (GetKey(KeyCode::Esc)) {
            Debug::System("SREngine::Run() : ESC key has been pressed.");
            break;
        }

        //===============================================
        if (m_compiler->HasNewScripts()) {
            m_compiler->CompileNewScripts();
            if (!is_awake_scripts) {
                m_compiler->AwakeNewScripts();
                is_awake_scripts = true;
            }
            m_compiler->StartNewScripts();
            m_compiler->ClearNewScripts();
        }
        //===============================================

        if (GetKey(KeyCode::W)) {
            m_camera->GetTransform()->Translate(0, 0, camera_speed, true);
        }
        else if (GetKey(KeyCode::S)) {
            m_camera->GetTransform()->Translate(0, 0, -camera_speed, true);
        }

        if (GetKey(KeyCode::A)) {
            m_camera->GetTransform()->Translate(-camera_speed, 0, 0, true);
        }
        else if (GetKey(KeyCode::D)) {
            m_camera->GetTransform()->Translate(camera_speed, 0, 0, true);
        }
    }

    return true;
}

bool SpaRcle::Engine::SREngine::Close() {
    if (!m_isRunning) {
        Debug::Error("Game engine is not running!");
        return false;
    } else Debug::Info("Close game engine...");

    this->m_isRunning = false;

    this->m_compiler->Destroy();

    this->m_graph->Close();

    Debug::System("All systems closed successfully!");

    return false;
}
