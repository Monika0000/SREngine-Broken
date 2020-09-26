#pragma once
#include <Debug.h>

//#pragma comment(lib,"lua.lib")
//extern "C" {
	//#include <lua.h>
	//#include <lualib.h>
	//#include <lauxlib.h>
//}

using namespace SpaRcle::Helper;

namespace SpaRcle {
	namespace Engine {
		class Script {
		private:
			std::string			m_lua_script_name	= "Unnamed";
			bool				m_isCompiled		= false;
			bool				m_isAwakened		= false;
			bool				m_isStarted			= false;
			bool				m_hasErrors			= false;
			//lua_State*			m_L					= nullptr;
			int					m_result			= -1;
		public:
			Script(std::string lua_script_name);
			~Script();
		public:
			bool Compile();
		public:
			bool Awake();
			bool Start();
			bool Update(float time);
			bool Close();
		};
	}
}

