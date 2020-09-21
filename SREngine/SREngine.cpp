#include "pch.h"
#include "SREngine.h"
#include <Input.h>

using namespace SpaRcle::Helper;
using namespace SpaRcle::Graphics;

bool SpaRcle::Engine::SREngine::Create(Window* win) {
    if (m_isCreated) {
        Debug::Error("Game engine already created!");
        return false;
    } else Debug::Info("Creating game engine...");

    this->m_graph = SRGraphics::Get();

    this->m_graph->Create(win);

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

    while (this->m_isRunning) {
        if (GetKey(KeyCode::Esc)) {
            Debug::System("SREngine::Run() : ESC key has been pressed.");
            break;
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

    this->m_graph->Close();

    return false;
}
