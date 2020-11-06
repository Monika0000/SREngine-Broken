#include "pch.h"
#include "SRString.h"
#include "Utils.h"

const std::string SpaRcle::Helper::SRString::VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
std::default_random_engine SpaRcle::Helper::SRString::generator = std::default_random_engine();
std::uniform_int_distribution<int> SpaRcle::Helper::SRString::distribution = std::uniform_int_distribution<int>(0, VALID_CHARS.size() - 1);

std::string SpaRcle::Helper::SRString::GetInBetweenStrings(std::string input, const std::string front, const std::string back) {
	size_t first = input.find(front) + 1;
	size_t last = input.find(back);
	if (first == SRMath::size_t_max)
		return "";
	else if (last == SRMath::size_t_max)
		return "";
	else
		return input.substr(first, last - first);
}