#include "pch.h"
#include "Render.h"
#include <Debug.h>

using namespace SpaRcle::Helper;

bool SpaRcle::Graphics::Render::Create() {
	if (m_isCreated) {
		Debug::Error("Render::Create() : render already created!");
		return false;
	} else Debug::Graph("Creating render...");

	this->m_canvas = new Canvas();

	this->m_isCreated = true;

	return true;
}

bool SpaRcle::Graphics::Render::Init() {
	if (m_isInit) {
		Debug::Error("Render::Init() : render already initialize!");
		return false;
	} else Debug::Graph("Initializing render...");

	this->m_isInit = true;

	return true;
}

bool SpaRcle::Graphics::Render::Run() {
	if (m_isRunning) {
		Debug::Error("Render::Run() : render already running!");
		return false;
	}
	else Debug::Graph("Running render...");

	this->m_isRunning = true;

	return true;
}

void SpaRcle::Graphics::Render::Close() {
	if (!m_isRunning) {
		Debug::Error("Render::Close() : render is not running!");
		return;
	} else Debug::Graph("Close render...");

	this->m_isRunning = false;

	return;
}

void SpaRcle::Graphics::Render::DrawSkybox() {
	this->m_skybox->Draw();
}

void SpaRcle::Graphics::Render::DrawGeometry() {

}

void SpaRcle::Graphics::Render::DrawGUI() {
	this->m_canvas->Draw();
}
