#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <random>
#include <iostream>

namespace SpaRcle {
	namespace Helper {
		class SRString {
		public:
			inline static float* SplitFloats(const char* source, char chr, 
				unsigned short start, unsigned short count_floats) 
			{
				float*			floats			= new float[count_floats];
				unsigned char   found_floats	= 0;

				unsigned short	count			= 0;
				unsigned short	found			= start;

				static short t = 0;
				static short len = 0;

				len = FastStrLen(source);

				for (t = start; t < len; t++) {
					count++;

					if (t + 1 == len) {
						if (found_floats + 1 == count_floats)
							return floats;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count);
						floats[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;

						//std::cout << ">>" << temp << "<<" << std::endl;
						return floats;
					} else if (source[t] == chr) {
						if (found_floats + 1 == count_floats)
							return floats;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count - 1);
						floats[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;
						//std::cout << ">>" << temp << "<<" << std::endl;

						found = t + 1;
						count = 0;
					}
				}
				return nullptr;
			}
			inline static const unsigned long FastStrLen(const char* str) noexcept {
				unsigned long len = 0;
				while (*str != '\0') {
					len++;
					str++;
				}
				return len;
			}
			inline static const wchar_t* CharsToWchar(const char* str) noexcept {
				const size_t cSize = strlen(str) + 1;
				wchar_t* wc = new wchar_t[cSize];
				mbstowcs(wc, str, cSize);
				return wc;
			}
			inline static std::string ReplaceAll(std::string const& original, std::string const& from, std::string const& to) noexcept {
				std::string results;
				std::string::const_iterator end = original.end();
				std::string::const_iterator current = original.begin();
				std::string::const_iterator next = std::search(current, end, from.begin(), from.end());
				while (next != end) {
					results.append(current, next);
					results.append(to);
					current = next + from.size();
					next = std::search(current, end, from.begin(), from.end());
				}
				results.append(current, next);
				return results;
			}
			inline static std::string ToLower(std::string str) noexcept {
				for (size_t t = 0; t < str.size(); t++)
					str[t] = tolower(str[t]);
				return str;
			}
			inline static std::string MakePath(std::string str) noexcept {
				str = SRString::ReplaceAll(str, "\\\\", "\\");
				str = SRString::ReplaceAll(str, "/", "\\");
				str = SRString::ToLower(str);
				return str;
			}
		};
	}
}