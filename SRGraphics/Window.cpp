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

#include "SRGraphics.h"

#include <glm\ext\matrix_clip_space.hpp>

//#include <sciter-x.h>
//#include <sciter-x-behavior.h>
//#include <sciter-x-dom.hpp>
 
#include <SRGUI.h>
#include <dwmapi.h>
#include <Input.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#pragma comment(lib,"Dwmapi.lib")

using namespace SpaRcle::Helper;

//static UINT SC_CALLBACK handle_notification(LPSCITER_CALLBACK_NOTIFICATION pnm, LPVOID callbackParam);

void Resize(GLFWwindow* window, int width, int height) {
	SpaRcle::Graphics::Window* win = SpaRcle::Graphics::SRGraphics::Get()->GetMainWindow();

	SpaRcle::Graphics::Window::FormatType type = win->GetFormat();

	glfwSetWindowSize(window, 
		SpaRcle::Graphics::Window::Format::GetWidth(type),
		SpaRcle::Graphics::Window::Format::GetHeight(type)
	);

	float ratio = 16.0 / 9.0;
	glMatrixMode(GL_PROJECTION);// используем матрицу проекции
	glLoadIdentity();// Reset матрицы
	glViewport(0, 0, 
		SpaRcle::Graphics::Window::Format::GetWidth(type),
		SpaRcle::Graphics::Window::Format::GetHeight(type)
	);// определяем окно просмотра
	gluPerspective(45, ratio, 0.1, 8000);// установить корректную перспективу.
	glMatrixMode(GL_MODELVIEW);// вернуться к модели

	win->GetProjection() = glm::perspective(glm::radians(45.f), ratio, 0.1f, 8000.0f);
}
void RePosition(GLFWwindow* window, int x, int y) {
	glfwSetWindowPos(window, x, y);
}

void SpaRcle::Graphics::Window::PoolEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		//std::cout << msg.message << " " << msg.wParam << std::endl;
		switch (msg.message)
		{
		case WM_CREATE: {
			//LRESULT res = SciterProcND(msg.hwnd, msg.message, msg.wParam, msg.lParam, NULL);
			break;
		}
		default:
			break;
		}

		switch (WindowEvents::GetEvent(msg.message, msg.wParam)) {
		case WindowEvents::Event::ALT_F4: {
			Debug::Log("Window::PoolEvents() : Alt+F4 has been pressed!");
			EventsManager::PushEvent(EventsManager::Event::Exit);
			glfwTerminate();
			break;
		}
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

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//LRESULT lResult;
	//BOOL    bHandled;

	//lResult = SciterProcND(hWnd, message, wParam, lParam, &bHandled);
	//if (!bHandled) {     // if it was handled by the Sciter
		//std::cout << message << " "  << wParam << " " << lParam << std::endl;
		//std::cout << "SciterProcND is OK\n";
		// return lResult; // then no further processing is required.
	//}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SpaRcle::Graphics::Window::InitWin32Window() {
	Debug::Graph("Window::InitWin32Window() : make window...");

	LPCUWSTR myclass = L"SpaRcleWindowClass";
	WNDCLASSEX wndclass = {
		sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
		0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
		LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
		0, myclass, LoadIcon(0,IDI_APPLICATION)
	};

	Debug::Graph("Window::InitWin32Window() : register window class...");

	if (!RegisterClassEx(&wndclass)) {
		Debug::Error("Window::InitWin32Window() : failed register window class!");
		return false;
	}
	this->m_hWnd = CreateWindowExW(0, myclass,
		SRString::CharsToWchar(m_win_name),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		Format::GetWidth(m_format), Format::GetHeight(m_format),
		0, 0,
		GetModuleHandle(0), 0
	);

	if (!m_hWnd) {
		Debug::Error("Window::InitWin32Window() : Failed find window! \n\tWindow hWnd is nullptr! Win name : " + std::string(m_win_name));
		this->m_isFailedRunning = true;
		return false;
	}
	else {
		Debug::Graph("Window::InitWin32Window() : hWnd has been found.");
	}
	m_hDC = GetDC(m_hWnd);

	DWM_BLURBEHIND bb;
	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = true;
	DwmEnableBlurBehindWindow(m_hWnd, &bb);

	return true;
}

bool SpaRcle::Graphics::Window::InitGlfw() {
	Debug::Graph("Initializing Glfw...");

	if (glfwInit()) {
		glfwWindowHint(GLFW_SAMPLES, 4); // 4x сглаживание
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWmonitor* glfw_monitor = NULL;// glfwGetPrimaryMonitor();

		this->m_glfw_window = glfwCreateWindow(
			Format::GetWidth(m_format),
			Format::GetHeight(m_format),
			m_win_name, glfw_monitor, nullptr
		);
		if (!m_glfw_window) {
			Debug::Error("Window::InitGlfw() : Failed to open GLFW window!");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_glfw_window);

		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.IniFilename = NULL;
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			//io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(m_glfw_window, true);
			ImGui_ImplOpenGL3_Init("#version 130");
		}

		//gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // ????? For GUI

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
		unsigned char* pxs = SOIL_load_image((ResourceManager::GetAbsoluteResourceFolder() + "\\Textures\\icon.png").c_str(),
			&icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
		if (pxs) {
			icons[0].pixels = pxs;
			glfwSetWindowIcon(m_glfw_window, 1, icons);
			SOIL_free_image_data(icons[0].pixels);
		}
		else
			Debug::Error("Failed loading ico image! Continue...\n\t" + (ResourceManager::GetAbsoluteResourceFolder() + "\\Textures\\icon.png"));

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
	//glfwSetWindowPosCallback(this->m_glfw_window, RePosition); //TODO: Re-position callback
	glfwShowWindow(this->m_glfw_window);

	glutInit(&this->m_argcp, &this->m_argv);

	return true;
}

bool SpaRcle::Graphics::Window::InitGL_Parametrs() {
	Debug::Graph("Initializing GL parametrs...");

	glEnable(GL_BLEND); // Прозрачность стекла
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glBlendEquation(GL_FUNC_ADD);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_MULTISAMPLE);

	///\%info ПРОВЕРЯЕМ ГЛУБИНУ, ЧТОБЫ ИЗБАВИТЬСЯ ОТ "ЭФФЕКТИА ПЕРЕКРЫТИЯ" ДАЛЬНИМИ ОБЪЕКТАМИ
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // Отсечение граней

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Действительно хорошие вычисления перспективы
	
	return true;
}

bool SpaRcle::Graphics::Window::Create() {
	Debug::Graph("Creating window...");

	this->m_screen_size = Window::GetScreenResolution();

	if (!m_render->Create(this)) { Debug::Error("Window::Create() : failed create render!"); return false; }
	if (!m_camera->Create(this)) { Debug::Error("Window::Create() : failed create camera!"); return false; }

	return true;
}
bool SpaRcle::Graphics::Window::Init() {
	Debug::Graph("Initializing window...");
	bool error = false;
	bool init = false;

	this->m_win_task = std::thread([&error, this, &init]() {
		if (!InitWin32Window()) {
			Debug::Error("Window::Init() : Failed to initialize win32!");
			error = true;
			return;
		}

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

		if (!m_render->Init()) {
			Debug::Error("Window::Init() : failed initialize render!");
			error = true;
			return;
		}
		if (!m_camera->Init()) {
			Debug::Error("Window::Init() : failed initialize camera!");
			error = true;
			return;
		}

		init = true;

		if (!RunOpenGLWindow()) {
			//error = true;
		}
		else {

		}
		});

	while (!init) {
		Sleep(1);
		if (error) {
			return false;
		}
		/* Wait initializing window... */
	}

	return true;
}
bool SpaRcle::Graphics::Window::Run() {
	Debug::Graph("Running window...");

	if (!m_render->Run()) { Debug::Error("Window::Run() : failed running render!"); return false; }
	if (!m_camera->Run()) { Debug::Error("Window::Run() : failed running camera!"); return false; }

	this->m_isRunning = true;

	while (!m_isWindowRun) {
		if (m_isFailedRunning)
			return false;
	}

	return true;
}

bool SpaRcle::Graphics::Window::RunOpenGLWindow() {
ret: if (!m_isRunning) goto ret; // Wait running window

	Debug::Graph("Running OpenGL window...");

	m_win32_hWnd = GraphUtils::FindWindowFromName(m_win_name);

	Resize(m_glfw_window, 0, 0);

	//SciterSetCallback(m_hWnd, handle_notification, NULL);

	this->m_isWindowRun = true;

	while (m_isRunning && !glfwWindowShouldClose(m_glfw_window)) {
		this->PoolEvents();

		this->m_render->PlayAnimators();

		this->Draw();

		glfwSwapBuffers(this->m_glfw_window);
	}

	Debug::Info("Window::RunOpenGLWindow() : window has been terminated!");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_render->Close();
	m_camera->Close();

	return true;
}

bool SpaRcle::Graphics::Window::IsFocusedWindow() { 
	return (m_win32_hWnd == GetForegroundWindow()); 
}

SpaRcle::Graphics::Window::Window(
	const char* win_name,
	int argcp, char* argv,
	Render* render,
	Camera* camera,
	FormatType format, 
	bool movable,
	bool mouseLock,
	bool vsync,
	unsigned char smooth_samples
) :
	m_win_name(win_name),
	m_smooth_samples(smooth_samples)
{
	this->m_argcp = argcp;
	this->m_argv = argv;

	this->m_render = render;
	this->m_camera = camera;

	this->m_format = format;
	this->m_movable = movable;
	this->MouseLock(mouseLock);
	this->m_vsync = vsync;

	this->m_camera_gm = GameObject::Instance("Main camera");
	this->m_camera_gm->AddComponent(m_camera);
};

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
SpaRcle::Graphics::GameObject* SpaRcle::Graphics::Window::GetCameraGameObject() {
	if (!m_camera_gm) {
		Debug::Error("Window::GetCameraGameObject() : camera game object is nullptr!");
		return nullptr;
	}
	else
		return m_camera_gm;
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

/*
// notifiaction cracker
UINT SC_CALLBACK handle_notification(LPSCITER_CALLBACK_NOTIFICATION pnm, LPVOID callbackParam)
{
	// Crack and call appropriate method
	// here are all notifiactions
	switch (pnm->code) {
	//case SC_LOAD_DATA: return on_load_data((LPSCN_LOAD_DATA)pnm);
	//case SC_DATA_LOADED: return on_data_loaded((LPSCN_DATA_LOADED)pnm);
	//case SC_ATTACH_BEHAVIOR: return attach_behavior((LPSCN_ATTACH_BEHAVIOR)pnm);
	//case SC_INVALIDATE_RECT: return on_invalidate_rect((LPSCN_INVALIDATE_RECT)pnm);
	case SC_ENGINE_DESTROYED: break;
	}
	return 0;
}*/
