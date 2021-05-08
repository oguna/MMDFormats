#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/RigidBody.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::RigidBody::RigidBody() noexcept
	: target_bone(0)
	, group(0)
	, mask(0)
	, shape(0)
	, mass(0.0f)
	, move_attenuation(0.0f)
	, rotation_attenuation(0.0f)
	, repulsion(0.0f)
	, friction(0.0f)
	, physics_calc_type(0)
{
	for (int i = 0; i < 3; ++i) {
		size[i] = 0.0f;
		position[i] = 0.0f;
		orientation[i] = 0.0f;
	}
}

void pmx::RigidBody::parse(std::istream& stream, Setting* setting)
{
	this->rigid_body_name = pmx::util::parseString(stream, setting->encoding);
	this->rigid_body_english_name = pmx::util::parseString(stream, setting->encoding);
	this->target_bone = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->group, sizeof(uint8_t));
	stream.read((char*)&this->mask, sizeof(uint16_t));
	stream.read((char*)&this->shape, sizeof(uint8_t));
	stream.read((char*)this->size, sizeof(float) * 3);
	stream.read((char*)this->position, sizeof(float) * 3);
	stream.read((char*)this->orientation, sizeof(float) * 3);
	stream.read((char*)&this->mass, sizeof(float));
	stream.read((char*)&this->move_attenuation, sizeof(float));
	stream.read((char*)&this->rotation_attenuation, sizeof(float));
	stream.read((char*)&this->repulsion, sizeof(float));
	stream.read((char*)&this->friction, sizeof(float));
	stream.read((char*)&this->physics_calc_type, sizeof(uint8_t));
}

std::size_t pmx::RigidBody::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->rigid_body_name, setting->encoding);
	total += pmx::util::dumpString(stream, this->rigid_body_english_name, setting->encoding);
	total += pmx::util::dumpIndex(stream, this->target_bone, setting->bone_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(&this->group)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->mask)), sizeof(uint16_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->shape)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(this->size)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->position)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->orientation)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->mass)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->move_attenuation)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->rotation_attenuation)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->repulsion)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->friction)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->physics_calc_type)), sizeof(uint8_t));

	total += sizeof(float) * 14 + sizeof(uint16_t) + sizeof(uint8_t) * 3;
	return total;
}
