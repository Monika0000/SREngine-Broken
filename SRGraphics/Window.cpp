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
 
#include <dwmapi.h>
#include <Input.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_draw.cpp>

#pragma comment(lib,"Dwmapi.lib")

using namespace SpaRcle::Helper;

//static UINT SC_CALLBACK handle_notification(LPSCITER_CALLBACK_NOTIFICATION pnm, LPVOID callbackParam);

GLuint loadTexture(unsigned char* pixels, int w, int h, int components)
{
	GLuint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, components, w, h, 0, (components == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	return textureID;
}

//void Resize(GLFWwindow* window, int w, int h) {
//	
//}
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
				//glfwTerminate();  
			}
			else {
				Debug::Log("Window::PoolEvents() : window closing canceled.");
			}
			break;
		default: DispatchMessage(&msg); break;
		}
	}
}

bool SpaRcle::Graphics::Window::ReCalcFBO(int width, int height) {
	if (!m_FBO) {
		glGenFramebuffers(1, &m_FBO);
		Debug::Graph("Window::ReCalcFBO() : frame buffer object has been created. Index : " + std::to_string(m_FBO));
	}

	if (!m_ScreenTexture) {
		glGenTextures(1, &m_ScreenTexture);
		Debug::Graph("Window::ReCalcFBO() : screen texture has been created. Index : " + std::to_string(m_ScreenTexture));
	}

	glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//================================================================================================

	if (m_ScreenTexture && m_FBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// присоедиение текстуры к объекту текущего кадрового буфера
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture, 0);
	}
	else {
		Debug::Error("Window::ReCalcFBO() : screen texture or FBO is not created!");
		EventsManager::PushEvent(EventsManager::Event::Error);
		return false;
	}

	if (!m_RBO) {
		glGenRenderbuffers(1, &m_RBO);
		Debug::Graph("Window::ReCalcFBO() : render buffer object has been created. Index : " + std::to_string(m_RBO));
	}

	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("Window::ReCalcFBO() : Framebuffer is not complete!\n\tWidth = " +
			std::to_string(width) + "\n\tHeight = " + std::to_string(height) + "\n\tScreenTexture = " + std::to_string(m_ScreenTexture) +
			"\n\tFBO = " + std::to_string(m_FBO) + "\n\tRBO = " + std::to_string(m_RBO));

		EventsManager::PushEvent(EventsManager::Event::Error);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
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
			
			{
				ImFont* pFont = io.Fonts->AddFontFromFileTTF((ResourceManager::GetAbsoluteResourceFolder() + "\\Fonts\\CalibriL.ttf").c_str(), 12.0f);

				unsigned char* pixels;
				int width, height, bytes_per_pixels;
				io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);
				GLuint id = loadTexture(pixels, width, height, 4);
				io.Fonts->SetTexID((void*)id);
			}

			//io.IniFilename = NULL;
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			//io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigDockingWithShift = true;
			io.ConfigWindowsResizeFromEdges = true;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			//io.ConfigDockingWithShift = true;
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(m_glfw_window, true);
			ImGui_ImplOpenGL3_Init("#version 130");

			//ImGui::DockContextInitialize(ImGui::GetCurrentContext());
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

		m_post_processing_shader = new Shader("post_processing", this->GetRender());
		if (!(m_post_processing_shader->Compile() && m_post_processing_shader->Linking())) {
			Debug::Error("Window::Init() : failed compiling/linking post processing shader!");
			error = true;
			return;
		}

		if (!m_post_processing->Init(
			m_post_processing_shader, 
			this->GetFBO(), 
			this->GetScreenTexture())
		) {
			Debug::Error("Window::Init() : failed initialize post processing!");
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

	//Resize(m_glfw_window, 0, 0);
	ResizeWindow(0, 0);

	//SciterSetCallback(m_hWnd, handle_notification, NULL);

	this->m_isWindowRun = true;

	double previousTime = glfwGetTime();
	double currentTime  = glfwGetTime();
	int frameCount = 0;

	while (m_isRunning && !glfwWindowShouldClose(m_glfw_window)) {
		frameCount++; currentTime = glfwGetTime();
		if (currentTime - previousTime >= 1.0) {
			m_FPS = frameCount;

			frameCount = 0;
			previousTime = currentTime;
		}

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

	if (m_post_processing) {
		this->m_post_processing->Destoy();
		delete m_post_processing;
	}

	if (m_post_processing_shader) {
		m_post_processing_shader->Destroy();
		delete m_post_processing_shader;
	}

	return true;
}

bool SpaRcle::Graphics::Window::IsFocusedWindow() { 
	return (m_win32_hWnd == GetForegroundWindow()); 
}

void SpaRcle::Graphics::Window::ResizeWindow(int w, int h) {
	SpaRcle::Graphics::Window::FormatType type = this->GetFormat();

	int format_width = SpaRcle::Graphics::Window::Format::GetWidth(type);
	int format_height = SpaRcle::Graphics::Window::Format::GetHeight(type);

	this->SetWindowFormatSize(format_width, format_height);

	glfwSetWindowSize(m_glfw_window,
		format_width,
		format_height
	);

	this->ReCalcFBO(format_width, format_height);

	this->GetPostProcessing()->Resize(format_width, format_height);

	//if (this->GetPostProcessing()->IsEnabledRenderIntoWindow()) {
		float ratio = 16.0 / 9.0;
		glMatrixMode(GL_PROJECTION);// используем матрицу проекции
		glLoadIdentity();// Reset матрицы
		glViewport(0, 0,
			format_width,
			format_height
		);// определяем окно просмотра
		gluPerspective(45, ratio, 0.1, 8000);// установить корректную перспективу.
		glMatrixMode(GL_MODELVIEW);// вернуться к модели

		this->GetProjection() = glm::perspective(glm::radians(45.f), ratio, 0.1f, 8000.0f);
	//}
}

/*
void SpaRcle::Graphics::Window::ResizeSceneWindow(int w, int h) {
	if (this->GetPostProcessing()->IsEnabledRenderIntoWindow()) return;

	//if (!child_window_size.x || !child_window_size.y)
	//	child_window_size = { w,h };
	
	this->ReCalcFBO(w, h);

	this->GetPostProcessing()->Resize(w, h);

	float ratio = 16 / 9;
	glMatrixMode(GL_PROJECTION);// используем матрицу проекции
	glLoadIdentity();// Reset матрицы
	glViewport(0, 0,
		w,
		h
	);// определяем окно просмотра
	gluPerspective(45, ratio, 0.1, 8000);// установить корректную перспективу.
	glMatrixMode(GL_MODELVIEW);// вернуться к модели

	this->GetProjection() = glm::perspective(glm::radians(45.f), ratio, 0.1f, 8000.0f);
}
*/

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

	this->m_post_processing = new PostProcessing(this);

	this->m_camera_gm = GameObject::Instance("Main camera");

	this->m_camera_gm->AddComponent(m_camera);
	this->m_camera_gm->AddComponent(m_post_processing);
}
ImVec2 SpaRcle::Graphics::Window::GetDockSpace() {
	return { m_window_format_size.x, m_window_format_size.y };
};

void SpaRcle::Graphics::Window::Draw() {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(
		GL_COLOR_BUFFER_BIT | // color   buffer
		GL_DEPTH_BUFFER_BIT | // depth   buffer
		GL_STENCIL_BUFFER_BIT // stencil buffer
	);

	this->m_render->PoolEvents();

	this->m_render->FindAimedMesh();

	this->m_post_processing->Begin();
	{
		this->m_render->DrawSkybox();

		this->m_render->DrawGeometry();
	}
	this->m_post_processing->End();

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
