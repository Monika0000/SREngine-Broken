#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>	
#include <random>
#include <ostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <Psapi.h>

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

namespace SpaRcle {
	namespace Helper {
		class Utils {
		private:
			Utils() {};
			~Utils() {};
		public:
			static const long long GetUsedMemoryLoad() noexcept {
				PROCESS_MEMORY_COUNTERS pmc;
				BOOL result = GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
				if (result)
					return (long long)pmc.PeakWorkingSetSize;
				else
					return -1;
			}
		};
	}
}

