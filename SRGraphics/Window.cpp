#include "pch.h"
#include "Window.h"
#include <Debug.h>
#include <GL/glut.h>
#include <CommCtrl.h>
#include <WinUser.h>

#include "WindowEvents.h"
#include <EventsManager.h>

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Window::Draw() {
	//this->m_camera->
}

void SpaRcle::Graphics::Window::PoolEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		//std::cout << msg.message << " " << msg.wParam << std::endl;

		switch (WindowEvents::GetEvent(msg.message, msg.wParam)) {
		case WindowEvents::Event::Close:
			if (MessageBox(NULL, L"Do you want to exit?", L"Exit?", MB_YESNO) == IDYES) {
				Debug::Log("Window::PoolEvents() : terminate window...");
				EventsManager::PushEvent(EventsManager::Event::Exit);
				glfwTerminate();
			}
			else {
				Debug::Log("Window::PoolEvents() : window closing canceled.");
			}
			break;
		default: DispatchMessage(&msg); break;
		}
	}
}

bool SpaRcle::Graphics::Window::InitGlfw() {
	Debug::Graph("Initializing Glfw...");
	if (glfwInit()) {
		glfwWindowHint(GLFW_SAMPLES, 4); // 4x сглаживание

		this->m_glfw_window = glfwCreateWindow(
			Format::GetWidth(m_format), 
			Format::GetHeight(m_format), 
			m_win_name, nullptr, nullptr
		);
		if (!m_glfw_window) {
			fprintf(stderr, "Failed to open GLFW window\n");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_glfw_window);

		const GLubyte* vendor		= glGetString(GL_VENDOR);	// Returns the vendor
		const GLubyte* renderer		= glGetString(GL_RENDERER); // Returns a hint to the model
		const GLubyte* version		= glGetString(GL_VERSION);	// Returns a hint to the model

		Debug::Info("Window::InitGlfw() : GL_VENDOR   is " + std::string((char*)vendor));
		Debug::Info("Window::InitGlfw() : GL_RENDERER is " + std::string((char*)renderer));
		Debug::Info("Window::InitGlfw() : GL_VERSION  is " + std::string((char*)version));

		glfwSetWindowPos(m_glfw_window,
			m_screen_size.x / 2 - Format::GetWidth(m_format)	/ 2,
			m_screen_size.y / 2 - Format::GetHeight(m_format)	/ 2);

		Debug::Graph("Window::InitGlfw() : Setting window icon...");

		//GLFWimage icons[1];
		//unsigned char* pxs = SOIL_load_image((resources_folder + "\\icon.png").c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
		//if (pxs) {
		//	icons[0].pixels = pxs;
		//	glfwSetWindowIcon(window, 1, icons);
		//	SOIL_free_image_data(icons[0].pixels);
		//}
		//else debug->Error("Failed loading ico image! Continue...\n\t" + (resources_folder + "\\icon.png"));

		return true;
	}
	else
		return false;
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
	Debug::Graph("Initializing GL parametrs...");

	return true;
}

bool SpaRcle::Graphics::Window::RunOpenGLWindow() {
ret: if (!m_isRunning) goto ret; // Wait running window

	Debug::Graph("Running OpenGL window...");

	this->m_isWindowRun = true;

	while (m_isRunning && !glfwWindowShouldClose(m_glfw_window)) {
		this->PoolEvents();

		this->Draw();

		glfwSwapBuffers(this->m_glfw_window);
	}

	return true;
}

bool SpaRcle::Graphics::Window::Create() {
	Debug::Graph("Creating window...");

	this->m_screen_size = Window::GetScreenResolution();

	return true;
}

bool SpaRcle::Graphics::Window::Init() {
	Debug::Graph("Initializing window...");
	bool error = false;
	bool init  = false;

	this->m_win_task = std::thread([&error, this, &init]() {
		if (!InitGlfw()) {
			Debug::Error("Window::Init() : Failed to initialize Glfw!");
			error = true;
			return;
		}

		if (!InitGlew()) {
			Debug::Error("Window::Init() : Failed to initialize Glew!");
			error = true;
			return;
		}

		if (!InitGlut()) {
			Debug::Error("Window::Init() : Failed to initialize Glut!");
			error = true;
			return;
		}

		if (!InitGL_Parametrs()) {
			Debug::Error("Window::Init() : Failed to initialize GL parametrs!");
			error = true;
			return;
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

	this->m_isRunning = false;

	this->m_win_task.join();

	return;
}
