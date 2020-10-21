#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <random>
#include <iostream>

namespace SpaRcle {
	namespace Helper {
		class SRString {
		public:
			static std::string GetExtensionFromFilePath(std::string path) {
				size_t size = path.size() - 1;
				std::string result = ""; 
				bool found = false;
				for (long long l = size; l >= 0; l--) {
					if (path[l] == '/' || path[l] == '\\')
						break;
					else if (path[l] == '.') {
						found = true;
						break;
					}
					else
						result += path[l];
				}

				if (!found) return "";

				result = SRString::Reverse(result);
				return result;
			}
			static std::string Reverse(std::string str) {
				std::string result = "";
				size_t size = str.size();
				for (size_t t = size; t > 0; t--) {
					result += str[t - 1];
				}

				return result;
			}
			static std::string GetInBetweenStrings(std::string input, const std::string front, const std::string back);
			static std::string Remove(const std::string str, char c) {
				std::string res = "";
				for (auto& a : str)
					if (a == c)
						break;
					else
						res += a;
				return res;
			}
			inline static std::string FromCharVector(const std::vector<char>& vs) noexcept {
				std::string result(begin(vs), end(vs));
				return result;
			}
			inline static const unsigned char MathCount(const char* str, char symb) noexcept {
				unsigned char count = 0;
				while (*str != '\0') {
					if (*str == symb)
						count++;
					str++;
				}
				return count;
			}
			static std::vector<std::string> Split(std::string str, std::string delimiter) {
				std::vector<std::string> result = {};

				size_t pos = 0;
				std::string token;
				while ((pos = str.find(delimiter)) != std::string::npos) {
					token = str.substr(0, pos);
					//std::cout << token << std::endl;
					if (token!="")
						result.push_back(token);
					str.erase(0, pos + delimiter.length());
				}

				return result;
			}
			static std::string ArrayToLine(std::vector<std::string> arr, std::string space) {
				std::string str = "";
				size_t size = arr.size();
				for (size_t t = 0; t < size; t++) {
					str += arr[t];
					if (t + 1 != size)
						str += space;
				}
				return str;
			}
			inline static char** Split(const char* source, char chr,
				unsigned short start, unsigned short count_strs)
			{
				char** strs = new char*[count_strs];
				unsigned char   found_floats = 0;

				unsigned short	count = 0;
				unsigned short	found = start;

				static short t = 0;
				static short len = 0;

				len = FastStrLen(source);

				for (t = start; t < len; t++) {
					count++;

					if (t + 1 == len) {
						if (found_floats == count_strs)
							return strs;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count);

						strs[found_floats] = temp;
						found_floats++;

						//delete[] temp;

						return strs;
					}
					else if (source[t] == chr) {
						if (found_floats + 1 == count_strs)
							return strs;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count - 1);

						strs[found_floats] = temp;
						found_floats++;

						//delete[] temp;

						found = t + 1;
						count = 0;
					}
				}
				return nullptr;
			}
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

					//std::cout << t << " " << len << std::endl;

					if (t + 1 == len) {
						//std::cout << ((int)found_floats) << std::endl;

						if (found_floats == count_floats)
							return floats;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count);
						//std::cout << ">>" << temp << "<<" << std::endl;

						floats[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;

						return floats;
					} else if (source[t] == chr) {
						if (found_floats + 1 == count_floats)
							return floats;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count - 1);
						//std::cout << ">>" << temp << "<<" << std::endl;

						floats[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;

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
			static std::string GetFileNameFromPath(std::string path) {
				std::reverse(path.begin(), path.end());
				std::string file = "";

				bool f = false;
				for (char c : path) {
					if (!f) {
						if (c == '.') f = true;
					}
					else {
						if (c == '\\' || c == '/')
							break;
						else
							file += c;
					}
				}

				std::reverse(file.begin(), file.end());
				return file;
			}

			//static std::string Substring(std::string str, unsigned int i) {
			//	int idx = 0;
			//	for (char c : str)
			//
			//}
			static std::string BackReadToChar(const std::string& str, const char c) {
				std::string nstr = std::string();

				for (size_t t = str.size(); t > 0; t--) {
					if (str[t - 1] == c)
						break;
					else nstr.push_back(str[t - 1]);
				}

				std::string reversed(nstr.rbegin(), nstr.rend());

				return reversed;
			}
		};
	}
}