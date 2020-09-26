#pragma once
#include "Utils.h"

namespace SpaRcle {
	namespace Helper {
		class Debug {
			enum class ConsoleColor {
				Black = 0,
				Blue = 1,
				Green = 2,
				Cyan = 3,
				Red = 4,
				Magenta = 5,
				Brown = 6,
				LightGray = 7,
				DarkGray = 8,
				LightBlue = 9,
				LightGreen = 10,
				LightCyan = 11,
				LightRed = 12,
				LightMagenta = 13,
				Yellow = 14,
				White = 15
			};
			enum class Type {
				Log, Info, Debug, Graph, Shader, System, Warn, Error
			};
		private:
			Debug() { };
			~Debug() { };
		private:
			inline static HANDLE		m_console					= nullptr;
			inline static bool			m_show_use_memory			= false;
			inline static bool			m_ColorThermeIsEnabled		= false;
			inline static volatile bool m_isInit					= false;
			inline static std::string	m_log_path					= "";
		private:
			static inline void InitColorTherme() {
				if (!m_ColorThermeIsEnabled)
					system("color 70");
				m_ColorThermeIsEnabled = true;
			}
			static void Print(std::string& msg, Type type);
		public:
			void Init(std::string log_path, bool ShowUsedMemory) {
				setlocale(LC_ALL, "rus");
				setlocale(LC_NUMERIC, "C");

				InitColorTherme();

				this->m_console				= GetStdHandle(STD_OUTPUT_HANDLE);

				this->m_log_path			= log_path;
				this->m_isInit				= true;
				this->m_show_use_memory		= ShowUsedMemory;

				std::string msg = "Debugger has been initialized.";
				Print(msg, Type::Debug);
			}
			void Stop() {
				std::string msg = "Debugger has been stopped.";
				Print(msg, Type::Debug);
			}
		public:
			static Debug* Get() {
				static Debug* debug = nullptr;
				if (!debug) {
					debug = new Debug();
				}
				return debug;
			}
		public:
			static void Log(std::string msg)		{ Print(msg, Type::Log);		}
			static void Info(std::string msg)		{ Print(msg, Type::Info);		}
			static void Graph(std::string msg)		{ Print(msg, Type::Graph);		}
			static void Shader(std::string msg)		{ Print(msg, Type::Shader);		}
			static void System(std::string msg)		{ Print(msg, Type::System);		}
			static void Warn(std::string msg)		{ Print(msg, Type::Warn);		}
			static void Error(std::string msg)		{ Print(msg, Type::Error);		}
		};
	}
}

