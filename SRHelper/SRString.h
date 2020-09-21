#pragma once
#include <string>
#include <random>

namespace SpaRcle {
	namespace Helper {
		class SRString {
		public:
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