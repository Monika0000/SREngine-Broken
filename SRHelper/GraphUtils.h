#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SRString.h"
#include <Windows.h>

namespace SpaRcle {
	namespace Helper {
		class GraphUtils {
		public:
			static const HWND FindWindowFromName(const char* name) noexcept {
				return FindWindowW(NULL, SRString::CharsToWchar(name));
			}
			static const std::string CkeckOpenGLErrors() noexcept {
				DWORD err = GetLastError();
				switch (err) {
					case 0:			return "";
					case 6:			return "The handle is invalid.";
					case 127:		return "The specified procedure could not be found.";
					case 1813:		return "The specified resource type cannot be found in the image file.";
					default:		return "Unknown code system error : " + std::to_string(err);
				}
			}
		};
	}
}

