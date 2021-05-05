/* strconv.h v1.8.1                */
/* Last Modified: 2021/04/18 06:04 */
#ifndef STRCONV_H
#define STRCONV_H

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#if __cplusplus >= 201103L && !defined(STRCONV_CPP98)
static inline std::wstring cp_to_wide(const std::string &s, UINT codepage)
{
  int in_length = (int)s.length();
  int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
  std::wstring result(out_length, L'\0');
  if (out_length)
    MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0], out_length);
  return result;
}
static inline std::string wide_to_cp(const std::wstring &s, UINT codepage)
{
  int in_length = (int)s.length();
  int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
  std::string result(out_length, '\0');
  if (out_length)
    WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0], out_length, 0, 0);
  return result;
}
#else /* __cplusplus < 201103L */
static inline std::wstring cp_to_wide(const std::string &s, UINT codepage)
{
  int in_length = (int)s.length();
  int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
  std::vector<wchar_t> buffer(out_length);
  if (out_length)
    MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &buffer[0], out_length);
  std::wstring result(buffer.begin(), buffer.end());
  return result;
}
static inline std::string wide_to_cp(const std::wstring &s, UINT codepage)
{
  int in_length = (int)s.length();
  int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
  std::vector<char> buffer(out_length);
  if (out_length)
    WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &buffer[0], out_length, 0, 0);
  std::string result(buffer.begin(), buffer.end());
  return result;
}
#endif

static inline std::string cp_to_utf8(const std::string &s, UINT codepage)
{
  if (codepage == CP_UTF8)
    return s;
  std::wstring wide = cp_to_wide(s, codepage);
  return wide_to_cp(wide, CP_UTF8);
}
static inline std::string utf8_to_cp(const std::string &s, UINT codepage)
{
  if (codepage == CP_UTF8)
    return s;
  std::wstring wide = cp_to_wide(s, CP_UTF8);
  return wide_to_cp(wide, codepage);
}

static inline std::wstring ansi_to_wide(const std::string &s)
{
  return cp_to_wide(s, CP_ACP);
}
static inline std::string wide_to_ansi(const std::wstring &s)
{
  return wide_to_cp(s, CP_ACP);
}

static inline std::wstring sjis_to_wide(const std::string &s)
{
  return cp_to_wide(s, 932);
}
static inline std::string wide_to_sjis(const std::wstring &s)
{
  return wide_to_cp(s, 932);
}

static inline std::wstring utf8_to_wide(const std::string &s)
{
  return cp_to_wide(s, CP_UTF8);
}
static inline std::string wide_to_utf8(const std::wstring &s)
{
  return wide_to_cp(s, CP_UTF8);
}

static inline std::string ansi_to_utf8(const std::string &s)
{
  return cp_to_utf8(s, CP_ACP);
}
static inline std::string utf8_to_ansi(const std::string &s)
{
  return utf8_to_cp(s, CP_ACP);
}

static inline std::string sjis_to_utf8(const std::string &s)
{
  return cp_to_utf8(s, 932);
}
static inline std::string utf8_to_sjis(const std::string &s)
{
  return utf8_to_cp(s, 932);
}

#ifdef __cpp_char8_t
static inline std::u8string utf8_to_char8(const std::string &s)
{
  return std::u8string(s.begin(), s.end());
}
static inline std::string char8_to_utf8(const std::u8string &s)
{
  return std::string(s.begin(), s.end());
}

static inline std::wstring char8_to_wide(const std::u8string &s)
{
  return cp_to_wide(char8_to_utf8(s), CP_UTF8);
}
static inline std::u8string wide_to_char8(const std::wstring &s)
{
  return utf8_to_char8(wide_to_cp(s, CP_UTF8));
}

static inline std::u8string cp_to_char8(const std::string &s, UINT codepage)
{
  return utf8_to_char8(cp_to_utf8(s, codepage));
}
static inline std::string char8_to_cp(const std::u8string &s, UINT codepage)
{
  return utf8_to_cp(char8_to_utf8(s), codepage);
}

static inline std::u8string ansi_to_char8(const std::string &s)
{
  return cp_to_char8(s, CP_ACP);
}
static inline std::string char8_to_ansi(const std::u8string &s)
{
  return char8_to_cp(s, CP_ACP);
}

static inline std::u8string sjis_to_char8(const std::string &s)
{
  return cp_to_char8(s, 932);
}
static inline std::string char8_to_sjis(const std::u8string &s)
{
  return char8_to_cp(s, 932);
}
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

static inline std::wstring vformat(const wchar_t *format, va_list args)
{
  int len = _vsnwprintf(0, 0, format, args);
  if (len < 0)
    return L"";
  std::vector<wchar_t> buffer(len + 1);
  len = _vsnwprintf(&buffer[0], len, format, args);
  if (len < 0)
    return L"";
  buffer[len] = L'\0';
  return &buffer[0];
}
static inline std::string vformat(const char *format, va_list args)
{
  int len = _vsnprintf(0, 0, format, args);
  if (len < 0)
    return "";
  std::vector<char> buffer(len + 1);
  len = _vsnprintf(&buffer[0], len, format, args);
  if (len < 0)
    return "";
  buffer[len] = '\0';
  return &buffer[0];
}
#ifdef __cpp_char8_t
static inline std::string vformat(const char8_t *format, va_list args)
{
  int len = _vsnprintf(0, 0, (const char *)format, args);
  if (len < 0)
    return "";
  std::vector<char> buffer(len + 1);
  len = _vsnprintf(&buffer[0], len, (const char *)format, args);
  if (len < 0)
    return "";
  buffer[len] = '\0';
  return &buffer[0];
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

static inline std::wstring format(const wchar_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::wstring s = vformat(format, args);
  va_end(args);
  return s;
}
static inline std::string format(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  return s;
}
#ifdef __cpp_char8_t
static inline std::string format(const char8_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  return s;
}
#endif

static inline void format(std::ostream &ostrm, const wchar_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::wstring s = vformat(format, args);
  va_end(args);
  ostrm << wide_to_utf8(s) << std::flush;
}
static inline void format(std::ostream &ostrm, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  ostrm << s << std::flush;
}
#ifdef __cpp_char8_t
static inline void format(std::ostream &ostrm, const char8_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  ostrm << s << std::flush;
}
#endif

static inline std::string formatA(const wchar_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::wstring s = vformat(format, args);
  va_end(args);
  return wide_to_ansi(s);
}
static inline std::string formatA(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  return utf8_to_ansi(s);
}
#ifdef __cpp_char8_t
static inline std::string formatA(const char8_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  return utf8_to_ansi(s);
}
#endif

static inline void formatA(std::ostream &ostrm, const wchar_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::wstring s = vformat(format, args);
  va_end(args);
  ostrm << wide_to_ansi(s) << std::flush;
}
static inline void formatA(std::ostream &ostrm, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  ostrm << utf8_to_ansi(s) << std::flush;
}
#ifdef __cpp_char8_t
static inline void formatA(std::ostream &ostrm, const char8_t *format, ...)
{
  va_list args;
  va_start(args, format);
  std::string s = vformat(format, args);
  va_end(args);
  ostrm << utf8_to_ansi(s) << std::flush;
}
#endif

class unicode_ostream
{
private:
  std::ostream *m_ostrm;
  UINT m_target_cp;
  bool is_ascii(const std::string &s)
  {
    for (std::size_t i = 0; i < s.size(); i++)
    {
      unsigned char c = (unsigned char)s[i];
      if (c > 0x7f)
        return false;
    }
    return true;
  }

public:
  unicode_ostream(std::ostream &ostrm, UINT target_cp = CP_ACP) : m_ostrm(&ostrm), m_target_cp(target_cp) {}
  UINT target_cp() { return m_target_cp; }
  void target_cp(UINT cp) { m_target_cp = cp; }
  std::ostream &stream() { return *m_ostrm; }
  void stream(std::ostream &ostrm) { m_ostrm = &ostrm; }
  template <typename T>
  unicode_ostream &operator<<(const T &x)
  {
    std::ostringstream oss;
    oss << x;
    std::string output = oss.str();
    if (is_ascii(output))
    {
      (*m_ostrm) << x;
    }
    else
    {
      (*m_ostrm) << utf8_to_cp(output, m_target_cp);
    }
    return *this;
  }
  unicode_ostream &operator<<(const std::wstring &x)
  {
    (*m_ostrm) << wide_to_cp(x, m_target_cp);
    return *this;
  }
  unicode_ostream &operator<<(const wchar_t *x)
  {
    (*m_ostrm) << wide_to_cp(x, m_target_cp);
    return *this;
  }
  unicode_ostream &operator<<(const std::string &x)
  {
    (*m_ostrm) << utf8_to_cp(x, m_target_cp);
    return *this;
  }
  unicode_ostream &operator<<(const char *x)
  {
    (*m_ostrm) << utf8_to_cp(x, m_target_cp);
    return *this;
  }
#ifdef __cpp_char8_t
  unicode_ostream &operator<<(const std::u8string &x)
  {
    (*m_ostrm) << char8_to_cp(x, m_target_cp);
    return *this;
  }
  unicode_ostream &operator<<(const char8_t *x)
  {
    (*m_ostrm) << char8_to_cp(x, m_target_cp);
    return *this;
  }
#endif
  unicode_ostream &operator<<(std::ostream &(*pf)(std::ostream &)) // For manipulators...
  {
    (*m_ostrm) << pf;
    return *this;
  }
  unicode_ostream &operator<<(std::basic_ios<char> &(*pf)(std::basic_ios<char> &)) // For manipulators...
  {
    (*m_ostrm) << pf;
    return *this;
  }
};

#define U8(X) ((const char *)u8##X)
#define WIDE(X) (L##X)

#endif /* STRCONV_H */
