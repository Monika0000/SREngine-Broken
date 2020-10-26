#define NOMINMAX
#define GLEW_STATIC 1
#define WINDOWLESS 1
#define _CRT_SECURE_NO_WARNINGS

#include <SRGraphics.h>
#include <Debug.h>
#include <SRFile.h>
#include <Input.h>
#include <SREngine.h>
#include <SRString.h>
#include <ResourceManager.h>

#include <GL\glew.h>

#include <SRGUI.h>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "SOIL.lib")

using namespace SpaRcle::Graphics;
using namespace SpaRcle::Helper;
using namespace SpaRcle::Engine;
using namespace SpaRcle::GUI;

int main(int argcp, char* argv) {
	//while (true)
	//	std::cout << Input::GetKeyPressedId() << std::endl;

	Debug::Get()->Init(SRFile::GetPathToExe(), true);

	//std::string resource_path = SRString::MakePath(SRFile::GetPathToExe() + "/../Resources");
	ResourceManager::Init(); //resource_path

	SRGraphics* graph	= SRGraphics::Get();
	SREngine*   engine	= SREngine::Get();

	Skybox* skybox = ResourceManager::LoadSkybox("Mountains");
	
	Camera* camera = new Camera();

	Render* render = new Render(skybox, camera);

	Window* window = new Window(
		"SpaRcle Engine",					// displayed window name
		argcp, argv,						// standart c++ main args
		render,								// render
		camera,								// camera
		Window::FormatType::_1280_720,		// window format
		true,								// movable
		false,								// mouse lock
		false,								// vsync
		4									// smooth samples
	);

	engine->Create(
		window 
	);

	engine->Init();

	engine->Run();

	engine->Close();

	ResourceManager::Destroy();
	Debug::Get()->Stop();
	
	return 0;
}

/*
* 0xABABABAB : Used by Microsoft's HeapAlloc() to mark "no man's land" guard bytes after allocated heap memory
* 0xABADCAFE : A startup to this value to initialize all free memory to catch errant pointers
* 0xBAADF00D : Used by Microsoft's LocalAlloc(LMEM_FIXED) to mark uninitialised allocated heap memory
* 0xBADCAB1E : Error Code returned to the Microsoft eVC debugger when connection is severed to the debugger
* 0xBEEFCACE : Used by Microsoft .NET as a magic number in resource files
* 0xCCCCCCCC : Used by Microsoft's C++ debugging runtime library to mark uninitialised stack memory
* 0xCDCDCDCD : Used by Microsoft's C++ debugging runtime library to mark uninitialised heap memory
* 0xDDDDDDDD : Used by Microsoft's C++ debugging heap to mark freed heap memory
* 0xDEADDEAD : A Microsoft Windows STOP Error code used when the user manually initiates the crash.
* 0xFDFDFDFD : Used by Microsoft's C++ debugging heap to mark "no man's land" guard bytes before and after allocated heap memory
* 0xFEEEFEEE : Used by Microsoft's HeapFree() to mark freed heap memory
*/