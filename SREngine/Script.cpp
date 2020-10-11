#include "pch.h"
#include "Script.h"
#include <Debug.h>

using namespace SpaRcle::Helper;

SpaRcle::Engine::Script::Script(std::string lua_script_name) {
	this->m_lua_script_name = lua_script_name;
}

SpaRcle::Engine::Script::~Script() {

}

bool SpaRcle::Engine::Script::Compile() {
	Debug::Script("Script::Compile() : compiling script \"" + this->m_lua_script_name+ "\"...");

	return true;
}

bool SpaRcle::Engine::Script::Awake()
{
	return false;
}

bool SpaRcle::Engine::Script::Start()
{
	return false;
}

bool SpaRcle::Engine::Script::Update(float time)
{
	return false;
}

bool SpaRcle::Engine::Script::Destroy() {
	Debug::Script("Script::Destroy() : destroying script \"" + m_lua_script_name + "\"...");
	return true;
}
