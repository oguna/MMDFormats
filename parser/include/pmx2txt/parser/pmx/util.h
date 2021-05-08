#pragma once

#include <iostream>

namespace pmx
{
namespace util
{
	int parseIndex(std::istream& stream, int size);

	size_t dumpIndex(std::ostream& stream, const int index, int size);

	std::string parseString(std::istream& stream, uint8_t encoding);

	size_t dumpString(std::ostream& stream, const std::string& str, uint8_t encoding);
}
}