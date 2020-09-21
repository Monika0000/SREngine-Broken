#include "pch.h"
#include "Window.h"
#include <Debug.h>

using namespace SpaRcle::Helper;

bool SpaRcle::Graphics::Window::InitGlfw() {
	Debug::Graph("Initializing Glfw...");

	return true;
}

bool SpaRcle::Graphics::Window::InitGlew() {
	Debug::Graph("Initializing Glew...");

	return true;
}

bool SpaRcle::Graphics::Window::InitGlut() {
	Debug::Graph("Initializing Glut...");

	return true;
}

bool SpaRcle::Graphics::Window::InitGL_Parametrs() {
	return true;
}

bool SpaRcle::Graphics::Window::RunOpenGLWindow() {
ret: if (!m_isRunning) goto ret; // Wait running window

	Debug::Graph("Running OpenGL window...");

	this->m_isWindowRun = true;

	Sleep(1000);

	return true;
}

bool SpaRcle::Graphics::Window::Create() {
	Debug::Graph("Creating window...");
	return true;
}

bool SpaRcle::Graphics::Window::Init() {
	Debug::Graph("Initializing window...");
	bool error = false;
	bool init  = false;

	this->m_win_task = std::thread([&error, this, &init]() {
		{
			if (!InitGlfw()) {
				Debug::Error("Failed to initialize Glfw!");
				error = true;
				return;
			}
			
			if (!InitGlew()) {
				Debug::Error("Failed to initialize Glew!");
				error = true;
				return;
			}
			
			if (!InitGlut()) {
				Debug::Error("Failed to initialize Glut!");
				error = true;
				return;
			}
		}

		init = true;

		if (!RunOpenGLWindow()) {
			error = true;
		}
	});
	
	while (!init) {
		Sleep(1); 
		if (error) return false;
		/* Wait initializing window... */
	}

	return true;
}

bool SpaRcle::Graphics::Window::Run() {
	Debug::Graph("Running window...");
	this->m_isRunning = true;

	return true;
}

void SpaRcle::Graphics::Window::Close() {
	Debug::Graph("Close window...");
	this->m_win_task.join();
}
