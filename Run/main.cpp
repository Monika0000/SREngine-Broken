#define NOMINMAX
#define GLEW_STATIC 1

#include <SRGraphics.h>
#include <Debug.h>
#include <SRFile.h>
#include <Input.h>
#include <SREngine.h>

using namespace SpaRcle::Graphics;
using namespace SpaRcle::Helper;
using namespace SpaRcle::Engine;

int main() {
	/*while (true) {
		if (GetKeyDown(KeyCode::Esc))
			std::cout << "Down first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;

		if (GetKeyUp(KeyCode::Esc))
			std::cout << "Up first" << std::endl;
	}*/

	Debug::Get()->Init(SRFile::GetPathToExe(), true);
	SRGraphics* graph	= SRGraphics::Get();
	SREngine*   engine	= SREngine::Get();

	Window* window = new Window(
		Window::Format::_1600_900, 
		true, 
		false,
		true
	);

	engine->Create(window);

	engine->Init();

	engine->Run();

	engine->Close();

	Debug::Get()->Stop();
	return 0;
}