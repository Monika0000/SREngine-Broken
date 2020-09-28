#include "pch.h"
#include "Window.h"
#include <Debug.h>
#include <GL/glut.h>
#include <CommCtrl.h>
#include <WinUser.h>

#include "WindowEvents.h"
#include <EventsManager.h>
#include <SOIL\SOIL.h>

#include "ResourceManager.h"
#include "Render.h"
#include <GraphUtils.h>

using namespace SpaRcle::Helper;

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
			Debug::Error("Window::InitGlfw() : Failed to open GLFW window!");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_glfw_window);

		const GLubyte* vendor	= glGetString(GL_VENDOR);	// Returns the vendor
		const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
		const GLubyte* version	= glGetString(GL_VERSION);	// Returns a hint to the model

		Debug::Info("Window::InitGlfw() : GL_VENDOR   is " + std::string((char*)vendor));
		Debug::Info("Window::InitGlfw() : GL_RENDERER is " + std::string((char*)renderer));
		Debug::Info("Window::InitGlfw() : GL_VERSION  is " + std::string((char*)version));

		glfwSetWindowPos(m_glfw_window,
			m_screen_size.x / 2 - Format::GetWidth(m_format) / 2,
			m_screen_size.y / 2 - Format::GetHeight(m_format) / 2);

		Debug::Graph("Window::InitGlfw() : Setting window icon...");

		GLFWimage icons[1];
		unsigned char* pxs = SOIL_load_image((ResourceManager::GetResourceFolder() + "\\Textures\\icon.png").c_str(),
			&icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
		if (pxs) {
			icons[0].pixels = pxs;
			glfwSetWindowIcon(m_glfw_window, 1, icons);
			SOIL_free_image_data(icons[0].pixels);
		}
		else
			Debug::Error("Failed loading ico image! Continue...\n\t" + (ResourceManager::GetResourceFolder() + "\\Textures\\icon.png"));

		return true;
	}
	else
		return false;
	return true;
}

bool SpaRcle::Graphics::Window::InitGlew() {
	Debug::Graph("Initializing Glew...");

	glewExperimental = TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		Debug::Error("Window::InitGlew() : failed initializing glew!\n\tReason : " + GraphUtils::CkeckOpenGLErrors());
		return false;
	}

	return true;
}

bool SpaRcle::Graphics::Window::InitGlut() {
	Debug::Graph("Initializing Glut...");
	
	glfwSwapInterval(this->m_vsync);
	//TODO: Resize callback
	//TODO: Re-position callback
	glfwShowWindow(this->m_glfw_window);

	glutInit(&this->m_argcp, &this->m_argv);

	return true;
}

bool SpaRcle::Graphics::Window::InitGL_Parametrs() {
	Debug::Graph("Initializing GL parametrs...");

	return true;
}

bool SpaRcle::Graphics::Window::Create() {
	Debug::Graph("Creating window...");

	this->m_screen_size = Window::GetScreenResolution();

	if (!m_render->Create(this)) { Debug::Error("Window::Create() : failed create render!"); return false; }

	return true;
}

bool SpaRcle::Graphics::Window::Init() {
	Debug::Graph("Initializing window...");
	bool error = false;
	bool init = false;

	if (!m_render->Init()) { Debug::Error("Window::Init() : failed initialize render!"); return false; }

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

	if (!m_render->Run()) { Debug::Error("Window::Run() : failed running render!"); return false; }

	this->m_isRunning = true;

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

void SpaRcle::Graphics::Window::Draw() {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(
		GL_COLOR_BUFFER_BIT | // color   buffer
		GL_DEPTH_BUFFER_BIT | // depth   buffer
		GL_STENCIL_BUFFER_BIT // stencil buffer
	);

	this->m_render->DrawSkybox();

	this->m_render->DrawGeometry();

	this->m_render->DrawGUI();
}

void SpaRcle::Graphics::Window::Close() {
	Debug::Graph("Close window...");

	this->m_isRunning = false;

	this->m_win_task.join();

	return;
}
SpaRcle::Graphics::Camera* SpaRcle::Graphics::Window::GetCamera() {
	if (!m_camera) {
		Debug::Error("Window::GetCamera() : camera is nullptr!");
		return nullptr;
	}
	else
		return m_camera;
}

SpaRcle::Graphics::Render* SpaRcle::Graphics::Window::GetRender() {
	if (!m_render) {
		Debug::Error("Window::GetRender() : render is nullptr!");
		return nullptr;
	}
	else
		return m_render;
}
