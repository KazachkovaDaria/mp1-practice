#include "additional.hpp"
std::string to_lowercase(const std::string& initial) {
	std::string result = initial;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}