#include "pch.h"
#include "SRGraphics.h"
#include <Debug.h>

#include "ResourceManager.h"

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::SRGraphics::FixedUpdate() {
	Debug::Graph("SRGraphics::FixedUpdate() : run update function...");

	std::map<std::string, Video*>* videos = ResourceManager::GetVidesBuffer();

	while (this->m_isRunning) {
		for (auto vid : *videos) {
			vid.second->NextFrame();
		}
	}

	Debug::Graph("SRGraphics::FixedUpdate() : update function has been completed.");
}

bool SpaRcle::Graphics::SRGraphics::Create(Window* win, std::string resource_folder) {
	this->m_window = win;
	this->m_resource_folder = resource_folder;

	if (m_isCreated) {
		Debug::Error("Graphics engine already create!");
		return false;
	} else Debug::Graph("Creating graphics engine...");

	if (!m_window->Create()) { Debug::Error("Failed creating window!"); return false; }

	this->m_isCreated = true;

	return true;
}

bool SpaRcle::Graphics::SRGraphics::Init() {
	if (m_isInit) {
		Debug::Error("Graphics engine already initialize!");
		return false;
	} else Debug::Graph("Initializing graphics engine...");

	if (!m_window->Init()) { Debug::Error("Failed initializing window!"); return false; }

	this->m_isInit = true;

	return true;
}

bool SpaRcle::Graphics::SRGraphics::Run() {
	if (m_isRunning){
		Debug::Error("Graphics engine already running!");
		return false;	
	} else Debug::Graph("Running graphics engine...");

	if (!this->m_window->Run()) { Debug::Error("Failed running window!"); return false; }

	this->m_update_task = std::thread(&SRGraphics::FixedUpdate, this);

	this->m_isRunning = true;

	return true;
}

bool SpaRcle::Graphics::SRGraphics::Close() {
	if (!m_isRunning){
		Debug::Error("Graphics engine is not running!");
		return false;
	} else Debug::Graph("Close graphics engine...");

	this->m_isRunning = false;

	if (this->m_update_task.joinable()) this->m_update_task.join();

	this->m_window->Close();

	return true;
}
