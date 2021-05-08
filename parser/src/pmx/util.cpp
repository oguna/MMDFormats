#pragma once

#include "pmx2txt/parser/pmx/util.h"

#include "pmx2txt/parser/converter.h"

	int pmx::util::parseIndex(std::istream& stream, int size)
	{
		switch (size)
		{
		case 1:
		{
			uint8_t tmp8{};
			stream.read((char*)&tmp8, sizeof(uint8_t));
			if (255 == tmp8)
			{
				return -1;
			}
			else {
				return (int)tmp8;
			}
		}
		case 2:
		{
			uint16_t tmp16{};
			stream.read((char*)&tmp16, sizeof(uint16_t));
			if (65535 == tmp16)
			{
				return -1;
			}
			else {
				return (int)tmp16;
			}
		}
		case 4:
		{
			int tmp32{};
			stream.read((char*)&tmp32, sizeof(int));
			return tmp32;
		}
		default:
			return -1;
		}
	}

	size_t pmx::util::dumpIndex(std::ostream& stream, const int index, int size)
	{
		switch (size)
		{
		case 1: {
			const uint8_t data = static_cast<const uint8_t>(index);
			stream.write(static_cast<const char*>(static_cast<const void*>(&data)), sizeof(uint8_t));
			return stream.good() ? sizeof(uint8_t) : 0;
		}
		case 2:
		{
			const uint16_t data = static_cast<const uint16_t>(index);
			stream.write(static_cast<const char*>(static_cast<const void*>(&data)), sizeof(uint16_t));
			return stream.good() ? sizeof(uint16_t) : 0;
		}
		case 4:
		{
			stream.write(static_cast<const char*>(static_cast<const void*>(&index)), sizeof(int));
			return stream.good() ? sizeof(int) : 0;
		}
		default:
			throw std::runtime_error("No such index size");
		}
	}

	std::string pmx::util::parseString(std::istream& stream, uint8_t encoding)
	{
		int size{};
		stream.read((char*)&size, sizeof(int));
		std::vector<char> buffer;
		if (size == 0)
		{
			return std::string("");
		}
		buffer.resize(size);
		stream.read((char*)buffer.data(), size);
		if (encoding == 0)
		{
			// UTF16
			const auto tmp = pmx2txt::utf16_to_utf8(buffer);
			return std::move(std::string(tmp.cbegin(), tmp.cend()));
		}
		else
		{
			// UTF8
			return std::move(std::string(buffer.data(), size));
		}
	}

	size_t pmx::util::dumpString(std::ostream& stream, const std::string& str, uint8_t encoding)
	{
		std::vector<char> buf{};

		if (encoding == 0)
		{
			// UTF16
			buf = pmx2txt::utf8_to_utf16(std::vector<char>(str.cbegin(), str.cend()));
			const std::wstring hoge(reinterpret_cast<const wchar_t*>(buf.data()), buf.size()/2);
		}
		else
		{
			buf = std::vector<char>(str.cbegin(), str.cend());
		}
		int total = buf.size();
		stream.write(static_cast<char*>(static_cast<void*>(&total)), sizeof(int));
		stream.write(static_cast<char*>(static_cast<void*>(buf.data())), buf.size());
		return total + sizeof(int);
	}
