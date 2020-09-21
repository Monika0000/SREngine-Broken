#define NOMINMAX
#define GLEW_STATIC 1

#include <SRGraphics.h>
#include <Debug.h>
#include <SRFile.h>
#include <Input.h>
#include <SREngine.h>
#include <SRString.h>
#include <ResourceManager.h>

using namespace SpaRcle::Graphics;
using namespace SpaRcle::Helper;
using namespace SpaRcle::Engine;

int main(int argcp, char* argv) {
	/*while (true) {
		if (GetKeyDown(KeyCode::Esc))
			std::cout << "Down first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;
	}*/

	std::string resource_path = SRString::MakePath(SRFile::GetPathToExe() + "/../Resources");

	Debug::Get()->Init(SRFile::GetPathToExe(), true);
	ResourceManager::Get()->Init(resource_path);

	SRGraphics* graph	= SRGraphics::Get();
	SREngine*   engine	= SREngine::Get();
	
	Render* render = new Render();
	Camera* camera = new Camera();
	
	Window* window = new Window(
		"SpaRcle Engine",
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
		window, 
		resource_path
	);

	engine->Init();

	engine->Run();

	engine->Close();

	Debug::Get()->Stop();
	return 0;
}