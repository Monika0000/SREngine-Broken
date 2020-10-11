#include "pch.h"
#include "Compiler.h"
#include "Script.h"

bool SpaRcle::Engine::Compiler::Destroy() {
	Debug::Info("Compiler::Destroy() : destroying compiler...");

	this->DestroyAllScripts();

	return true;
}

bool SpaRcle::Engine::Compiler::CompileNewScripts() {
	for (unsigned int i = 0; i < m_count_new_scripts; i++) {
		if (m_new_scripts[0]->Compile()) {
			m_scripts.push_back(m_new_scripts[0]);
		}
		else
			m_new_scripts[0]->Destroy();
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
	this->m_count_new_scripts = 0;
	return true;
}

bool SpaRcle::Engine::Compiler::UpdateAllScripts() {
	return true;
}

bool SpaRcle::Engine::Compiler::DestroyAllScripts() {
	Debug::Info("Compiler::DestroyAllScripts() : destroying " + std::to_string(m_scripts.size()) + " scripts...");

	for (auto& script : this->m_scripts) {
		script->Destroy();
	}
	return true;
}

bool SpaRcle::Engine::Compiler::AddScript(Script* script) {
	this->m_new_scripts.push_back(script);
	this->m_count_new_scripts++;
	return true;
}

bool SpaRcle::Engine::Compiler::RecompileAll() {
	return true;
}
