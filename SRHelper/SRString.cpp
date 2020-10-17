#include "pch.h"
#include "SRString.h"
#include "Utils.h"

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