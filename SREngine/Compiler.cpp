#include "pch.h"
#include "Compiler.h"

bool SpaRcle::Engine::Compiler::Destroy() {
	return true;
}

bool SpaRcle::Engine::Compiler::CompileNewScripts() {
	for (unsigned int i = 0; i < m_count_new_scripts; i++) {
		if (m_new_scripts[0]->Compile()) {
			m_scripts.push_back(m_new_scripts[0]);
		}
		else
			delete m_new_scripts[0];
	}
	return true;
}

bool SpaRcle::Engine::Compiler::AwakeNewScripts() {
	return true;
}

bool SpaRcle::Engine::Compiler::StartNewScripts() {
	return true;
}

bool SpaRcle::Engine::Compiler::ClearNewScripts() {
	this->m_new_scripts.clear();
	return true;
}

bool SpaRcle::Engine::Compiler::UpdateAllScripts() {
	return true;
}

bool SpaRcle::Engine::Compiler::CloseAllScripts() {
	return true;
}

bool SpaRcle::Engine::Compiler::AddScript(Script* script) {
	return true;
}

bool SpaRcle::Engine::Compiler::RecompileAll() {
	return true;
}
