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

class SRFile {
public:
	inline static const std::string GetPathToExe() noexcept {
		TCHAR cwd[100];
		GetCurrentDirectory(100, cwd);
		std::wstring ws(&cwd[0]);
		return std::string(ws.begin(), ws.end());
	}
};

