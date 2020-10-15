#define NOMINMAX
#define GLEW_STATIC 1
#define _CRT_SECURE_NO_WARNINGS

#include <SRGraphics.h>
#include <Debug.h>
#include <SRFile.h>
#include <Input.h>
#include <SREngine.h>
#include <SRString.h>
#include <ResourceManager.h>

#include <GL\glew.h>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "SOIL.lib")

using namespace SpaRcle::Graphics;
using namespace SpaRcle::Helper;
using namespace SpaRcle::Engine;

int main(int argcp, char* argv) {

	/*
	while (true) {
		char** vs = SRString::Split("12345 qwerty 282828 0000", ' ', 0, 4);
		//float* vs = SRString::SplitFloats("||3333 12345 2.33", ' ', 2, 3);
		std::cout << vs[0] << std::endl;
		std::cout << vs[1] << std::endl;
		std::cout << vs[2] << std::endl;
		std::cout << vs[3] << std::endl;
		//std::cout << vs[0] << " " << vs[1] << " " << vs[2] << std::endl;


		delete[] vs[0];
		delete[] vs[1];
		delete[] vs[2];
		delete[] vs[3];
		delete[] vs;
	}*/

	/*while (true) {
		if (GetKeyDown(KeyCode::Esc))
			std::cout << "Down first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;
	}*/
	Debug::Get()->Init(SRFile::GetPathToExe(), true);

	std::string resource_path = SRString::MakePath(SRFile::GetPathToExe() + "/../Resources");
	ResourceManager::Init(resource_path);

	SRGraphics* graph	= SRGraphics::Get();
	SREngine*   engine	= SREngine::Get();

	Skybox* skybox = new Skybox(
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	); //TODO: Load skybox
	
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