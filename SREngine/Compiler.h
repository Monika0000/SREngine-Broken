#pragma once
#include "Script.h"

using namespace SpaRcle::Helper;

namespace SpaRcle {
	namespace Engine {
		class Compiler {
		public:
			Compiler() { };
			~Compiler() { };
		public:
			bool Destroy();
			bool RecompileAll();
		public:
			bool CompileNewScripts();
			bool AwakeNewScripts();
			bool StartNewScripts();
			bool ClearNewScripts();
		public:
			bool UpdateAllScripts();
		private:
			bool CloseAllScripts();
		public:
			bool AddScript(Script* script);
			inline const bool HasNewScripts() const noexcept { return m_count_new_scripts > 0; }
		private:
			unsigned int		 m_count_new_scripts		= 0;
			std::vector<Script*> m_new_scripts				= std::vector<Script*>();
			std::vector<Script*> m_scripts					= std::vector<Script*>();
		};
	}
}

