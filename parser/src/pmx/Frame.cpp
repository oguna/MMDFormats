#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Frame.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::FrameElement::FrameElement() noexcept
	: element_target(0)
	, index(0)
{
}

void pmx::FrameElement::parse(std::istream& stream, Setting* setting)
{
	stream.read((char*)&this->element_target, sizeof(uint8_t));
	if (this->element_target == 0x00)
	{
		this->index = pmx::util::parseIndex(stream, setting->bone_index_size);
	}
	else {
		this->index = pmx::util::parseIndex(stream, setting->morph_index_size);
	}
}

std::size_t pmx::FrameElement::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	stream.write(static_cast<char*>(static_cast<void*>(&this->element_target)), sizeof(uint8_t));
	total += sizeof(uint8_t);
	if (this->element_target == 0x00)
	{
		total += pmx::util::dumpIndex(stream, this->index, setting->bone_index_size);
	}
	else {
		total += pmx::util::dumpIndex(stream, this->index, setting->morph_index_size);
	}
	return total;
}

pmx::Frame::Frame() noexcept
	: frame_flag(0)
	, element_count(0)
{
}

void pmx::Frame::parse(std::istream& stream, Setting* setting)
{
	this->frame_name = pmx::util::parseString(stream, setting->encoding);
	this->frame_english_name = pmx::util::parseString(stream, setting->encoding);
	stream.read((char*)&this->frame_flag, sizeof(uint8_t));
	stream.read((char*)&this->element_count, sizeof(int));
	this->elements.resize(this->element_count);
	for (auto& item : this->elements)
	{
		item.parse(stream, setting);
	}
}

std::size_t pmx::Frame::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->frame_name, setting->encoding);
	total += pmx::util::dumpString(stream, this->frame_english_name, setting->encoding);


	stream.write(static_cast<char*>(static_cast<void*>(&this->frame_flag)), sizeof(uint8_t));
	total += sizeof(uint8_t);

	stream.write(static_cast<char*>(static_cast<void*>(&this->element_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->element_count; i++)
	{
		total += this->elements[i].dump(stream, setting);
	}
	return total;
}
