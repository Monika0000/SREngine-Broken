#include "pch.h"
#include "SRGraphics.h"
#include <Debug.h>

using namespace SpaRcle::Helper;

bool SpaRcle::Graphics::SRGraphics::Create(Window* win) {
	this->m_window = win;

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

	this->m_isRunning = true;

	return true;
}

bool SpaRcle::Graphics::SRGraphics::Close() {
	if (!m_isRunning){
		Debug::Error("Graphics engine is not running!");
		return false;
	} else Debug::Graph("Close graphics engine...");

	this->m_isRunning = false;
	
	this->m_window->Close();

	return true;
}
