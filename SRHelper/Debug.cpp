#include "pch.h"
#include "Debug.h"

using namespace SpaRcle::Helper;

void SpaRcle::Helper::Debug::Print(std::string& msg, Type type) {
	static bool use = false;
ret:
	if (!use) use = true; else goto ret;

	static std::string pref		= "";
	static ConsoleColor color	= ConsoleColor::Black;

	switch (type) {
		case SpaRcle::Helper::Debug::Type::Log:		pref = "[Log] ";	color = ConsoleColor::Cyan;				break;
		case SpaRcle::Helper::Debug::Type::Info:	pref = "[Info] ";	color = ConsoleColor::Magenta;			break;
		case SpaRcle::Helper::Debug::Type::Debug:	pref = "[Debug] ";	color = ConsoleColor::Blue;				break;
		case SpaRcle::Helper::Debug::Type::Graph:	pref = "[Graph] ";	color = ConsoleColor::Green;			break;
		case SpaRcle::Helper::Debug::Type::Shader:	pref = "[Shader] ";	color = ConsoleColor::LightCyan;		break;
		case SpaRcle::Helper::Debug::Type::System:	pref = "[System] ";	color = ConsoleColor::LightBlue;		break;
		case SpaRcle::Helper::Debug::Type::Warn:	pref = "[Warn] ";	color = ConsoleColor::Yellow;			break;
		case SpaRcle::Helper::Debug::Type::Error:	pref = "[Error] ";	color = ConsoleColor::LightRed;			break;
		default:									pref = "[Unk] ";	color = ConsoleColor::Black;			break;
	}

	if (Debug::m_show_use_memory)
		std::cout << '<' << Utils::GetUsedMemoryLoad() / 1024.f << " KB> ";
	
	SetConsoleTextAttribute(Debug::m_console, (WORD)(((int)ConsoleColor::LightGray << 4) | (int)color));
	std::cout << pref;
	SetConsoleTextAttribute(Debug::m_console, (WORD)(((int)ConsoleColor::LightGray << 4) | (int)ConsoleColor::Black));

	std::cout << msg << std::endl;

	use = false;
}
