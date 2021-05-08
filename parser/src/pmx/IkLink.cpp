#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/IkLink.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::IkLink::IkLink() noexcept
	: link_target(0)
	, angle_lock(0)
{
	for (int i = 0; i < 3; ++i) {
		max_radian[i] = 0.0f;
		min_radian[i] = 0.0f;
	}
}

void pmx::IkLink::parse(std::istream& stream, Setting* setting)
{
	this->link_target = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->angle_lock, sizeof(uint8_t));
	if (angle_lock == 1)
	{
		stream.read((char*)this->max_radian, sizeof(float) * 3);
		stream.read((char*)this->min_radian, sizeof(float) * 3);
	}
}

std::size_t pmx::IkLink::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->link_target, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->angle_lock)), sizeof(uint8_t));
	total += sizeof(uint8_t);
	if (angle_lock == 1)
	{
		stream.write(static_cast<char*>(static_cast<void*>(&this->max_radian)), sizeof(float) * 3);
		stream.write(static_cast<char*>(static_cast<void*>(&this->min_radian)), sizeof(float) * 3);
		total += sizeof(float) * 6;
	}
	return total;
}
