#include "pmx2txt/parser/converter.h"

#ifdef MSVC
#include "javacommons/strconv.h"

std::vector<char> pmx2txt::utf8_to_utf16(const std::vector<char>& source) noexcept
{
    std::wstring utf16str = utf8_to_wide(std::string(source.cbegin(), source.cend()));
    const size_t bufsize = utf16str.length() * (sizeof(wchar_t) / sizeof(char));
    std::vector<char> buf(bufsize);
    std::memcpy(buf.data(), utf16str.data(), bufsize);
    return std::move(buf);
}

std::vector<char> pmx2txt::utf16_to_utf8(const std::vector<char>& source) noexcept
{
    std::wstring tmpwstr(static_cast<const wchar_t*>(static_cast<const void*>(source.data())), source.size() / 2);
    const auto tmp = wide_to_utf8(tmpwstr);
    return std::move(std::vector<char>(tmp.cbegin(), tmp.cend()));
}

#else
#include <array>

#include <unicode/ucnv.h>
std::vector<char> pmx2txt::utf8_to_utf16(const std::vector<char>& source) noexcept
{
    UErrorCode err = U_ZERO_ERROR;
    std::array<char, 1024> buf;
    buf.fill(0);

    const auto r = ucnv_convert(
        "UTF-16LE",
        "UTF-8",
        buf.data(),
        buf.size(),
        source.data(),
        source.size(),
        &err
    );
    return std::move(std::vector<char>(buf.data(), buf.data()+r));
}

std::vector<char> pmx2txt::utf16_to_utf8(const std::vector<char>& source) noexcept
{
    UErrorCode err = U_ZERO_ERROR;
    std::array<char, 1024> buf;
    buf.fill(0);

    const auto r = ucnv_convert(
        "UTF-8",
        "UTF-16LE",
        buf.data(),
        buf.size(),
        source.data(),
        source.size(),
        &err
    );
    return std::move(std::vector<char>(buf.data(), buf.data()+r));
}

#endif
