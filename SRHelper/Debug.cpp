#include "pch.h"
#include "Debug.h"
#include "SRFile.h"

using namespace SpaRcle::Helper;

void SpaRcle::Helper::Debug::Init(std::string log_path, bool ShowUsedMemory) {
	setlocale(LC_ALL, "rus");
	setlocale(LC_NUMERIC, "C");

	InitColorTherme();

	this->m_log_path = log_path + "/log.txt";
	if (SRFile::FileExists(m_log_path))
		SRFile::Delete(m_log_path);
	this->m_file.open(m_log_path);

	this->m_console = GetStdHandle(STD_OUTPUT_HANDLE);

	this->m_isInit = true;
	this->m_show_use_memory = ShowUsedMemory;

	std::string msg = "Debugger has been initialized.";
	Print(msg, Type::Debug);
}

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
		case SpaRcle::Helper::Debug::Type::Script:	pref = "[Script] ";	color = ConsoleColor::Brown;			break;
		case SpaRcle::Helper::Debug::Type::System:	pref = "[System] ";	color = ConsoleColor::LightBlue;		break;
		case SpaRcle::Helper::Debug::Type::Warn:	pref = "[Warn] ";	color = ConsoleColor::Yellow;			break;
		case SpaRcle::Helper::Debug::Type::Error:	pref = "[Error] ";	color = ConsoleColor::LightRed;			break;
		default:									pref = "[Unk] ";	color = ConsoleColor::Black;			break;
	}

	if (Debug::m_show_use_memory) {
		std::cout << '<' << Utils::GetUsedMemoryLoad() / 1024.f << " KB> ";
		if (m_file.is_open())
			m_file << '<' << Utils::GetUsedMemoryLoad() / 1024.f << " KB> ";
	}
	
	SetConsoleTextAttribute(Debug::m_console, (WORD)(((int)ConsoleColor::LightGray << 4) | (int)color));
	std::cout << pref;
	if (m_file.is_open())
		m_file << pref;
	SetConsoleTextAttribute(Debug::m_console, (WORD)(((int)ConsoleColor::LightGray << 4) | (int)ConsoleColor::Black));

	std::cout << msg << std::endl;
	if (m_file.is_open())
		m_file << msg << std::endl;

	use = false;
}
