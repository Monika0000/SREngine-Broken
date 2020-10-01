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
		class SRMath {
		private:
			SRMath() {};
			~SRMath() {};
		public:
			inline static const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825;
			static const double RadToAngle(double rad) noexcept {
				return rad * PI / 180.0;
			}
			static const double AngleToRad(double angle) noexcept {
				return angle * 180.0 / PI;
			}
			inline static const size_t size_t_max = 18446744073709551615; //18446744073709551615
			inline static double Round(double x) { return ((x * 100) / 100); /*return floor(x * 100) / 100;*/ }
			inline static bool IsFractional(double value) { return !(value == round(value)); }
		};

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

