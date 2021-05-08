#ifndef PMX2TXT_PARSER_HPP_INCLUDED
#define PMX2TXT_PARSER_HPP_INCLUDED

#include <vector>

namespace pmx2txt{
    std::vector<char> utf8_to_utf16(const std::vector<char>& source) noexcept;
    std::vector<char> utf16_to_utf8(const std::vector<char>& source) noexcept;
}

#endif //PMX2TXT_PARSER_HPP_INCLUDED