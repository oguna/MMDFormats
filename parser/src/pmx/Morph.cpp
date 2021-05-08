#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Morph.h"

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/util.h"

pmx::MorphVertexOffset::MorphVertexOffset() noexcept
	: vertex_index(0)
{
	for (int i = 0; i < 3; ++i) {
		position_offset[i] = 0.0f;
	}
}

void pmx::MorphVertexOffset::parse(std::istream& stream, Setting* setting)
{
	this->vertex_index = pmx::util::parseIndex(stream, setting->vertex_index_size);
	stream.read((char*)this->position_offset, sizeof(float) * 3);
}

std::size_t pmx::MorphVertexOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->vertex_index, setting->vertex_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->position_offset)), sizeof(float) * 3);
	total += sizeof(float) * 3;
	return total;
}

pmx::MorphUVOffset::MorphUVOffset() noexcept
	: vertex_index(0)
{
	for (int i = 0; i < 4; ++i) {
		uv_offset[i] = 0.0f;
	}
}

void pmx::MorphUVOffset::parse(std::istream& stream, Setting* setting)
{
	this->vertex_index = pmx::util::parseIndex(stream, setting->vertex_index_size);
	stream.read((char*)this->uv_offset, sizeof(float) * 4);
}

std::size_t pmx::MorphUVOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->vertex_index, setting->vertex_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->uv_offset)), sizeof(float) * 4);
	total += sizeof(float) * 4;
	return total;
}

pmx::MorphBoneOffset::MorphBoneOffset() noexcept
	: bone_index(0)
{
	for (int i = 0; i < 3; ++i) {
		translation[i] = 0.0f;
	}
	for (int i = 0; i < 4; ++i) {
		rotation[i] = 0.0f;
	}
}

void pmx::MorphBoneOffset::parse(std::istream& stream, Setting* setting)
{
	this->bone_index = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)this->translation, sizeof(float) * 3);
	stream.read((char*)this->rotation, sizeof(float) * 4);
}

std::size_t pmx::MorphBoneOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->translation)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->rotation)), sizeof(float) * 4);
	total += sizeof(float) * 7;
	return total;
}

pmx::MorphMaterialOffset::MorphMaterialOffset() noexcept
	: specularity(0.0f)
	, edge_size(0.0f)
{
	for (int i = 0; i < 3; ++i) {
		specular[i] = 0.0f;
		ambient[i] = 0.0f;
	}
	for (int i = 0; i < 4; ++i) {
		diffuse[i] = 0.0f;
		edge_color[i] = 0.0f;
		texture_argb[i] = 0.0f;
		sphere_texture_argb[i] = 0.0f;
		toon_texture_argb[i] = 0.0f;
	}
}

void pmx::MorphMaterialOffset::parse(std::istream& stream, Setting* setting)
{
	this->material_index = pmx::util::parseIndex(stream, setting->material_index_size);
	stream.read((char*)&this->offset_operation, sizeof(uint8_t));
	stream.read((char*)this->diffuse, sizeof(float) * 4);
	stream.read((char*)this->specular, sizeof(float) * 3);
	stream.read((char*)&this->specularity, sizeof(float));
	stream.read((char*)this->ambient, sizeof(float) * 3);
	stream.read((char*)this->edge_color, sizeof(float) * 4);
	stream.read((char*)&this->edge_size, sizeof(float));
	stream.read((char*)this->texture_argb, sizeof(float) * 4);
	stream.read((char*)this->sphere_texture_argb, sizeof(float) * 4);
	stream.read((char*)this->toon_texture_argb, sizeof(float) * 4);
}

std::size_t pmx::MorphMaterialOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->material_index, setting->material_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->offset_operation)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->diffuse)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(&this->specular)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->specularity)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->ambient)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge_color)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge_size)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->texture_argb)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(&this->sphere_texture_argb)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(&this->toon_texture_argb)), sizeof(float) * 4);
	total += sizeof(float) * 28;
	return total;
}

pmx::MorphGroupOffset::MorphGroupOffset() noexcept
	: morph_index(0)
	, morph_weight(0.0f)
{}

void pmx::MorphGroupOffset::parse(std::istream& stream, Setting* setting)
{
	this->morph_index = pmx::util::parseIndex(stream, setting->morph_index_size);
	stream.read((char*)&this->morph_weight, sizeof(float));
}

std::size_t pmx::MorphGroupOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->morph_index, setting->morph_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::MorphFlipOffset::MorphFlipOffset() noexcept
	: morph_index(0)
	, morph_value(0.0f)
{}

void pmx::MorphFlipOffset::parse(std::istream& stream, Setting* setting)
{
	this->morph_index = pmx::util::parseIndex(stream, setting->morph_index_size);
	stream.read((char*)&this->morph_value, sizeof(float));
}

std::size_t pmx::MorphFlipOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->morph_index, setting->morph_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_value)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::MorphImpulseOffset::MorphImpulseOffset() noexcept
	: rigid_body_index(0)
	, is_local(0)
{
	for (int i = 0; i < 3; ++i) {
		velocity[i] = 0.0f;
		angular_torque[i] = 0.0f;
	}
}

void pmx::MorphImpulseOffset::parse(std::istream& stream, Setting* setting)
{
	this->rigid_body_index = pmx::util::parseIndex(stream, setting->rigidbody_index_size);
	stream.read((char*)&this->is_local, sizeof(uint8_t));
	stream.read((char*)this->velocity, sizeof(float) * 3);
	stream.read((char*)this->angular_torque, sizeof(float) * 3);
}

std::size_t pmx::MorphImpulseOffset::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->rigid_body_index, setting->rigidbody_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->is_local)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->velocity)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->angular_torque)), sizeof(float) * 3);
	total += sizeof(float) * 6 + sizeof(uint8_t);
	return total;
}

pmx::Morph::Morph() noexcept
	: offset_count(0)
{
}

void pmx::Morph::parse(std::istream& stream, Setting* setting)
{
	this->morph_name = pmx::util::parseString(stream, setting->encoding);
	this->morph_english_name = pmx::util::parseString(stream, setting->encoding);
	stream.read((char*)&category, sizeof(MorphCategory));
	stream.read((char*)&morph_type, sizeof(MorphType));
	stream.read((char*)&this->offset_count, sizeof(int));
	switch (this->morph_type)
	{
	case MorphType::Group:
		this->group_offsets.resize(this->offset_count);
		for(auto& item :this->group_offsets)
		{
			item.parse(stream, setting);
		}
		break;
	case MorphType::Vertex:
		this->vertex_offsets.resize(this->offset_count);
		for (auto& item : this->vertex_offsets)
		{
			item.parse(stream, setting);
		}
		break;
	case MorphType::Bone:
		this->bone_offsets.resize(this->offset_count);
		for (auto& item : this->bone_offsets)
		{
			item.parse(stream, setting);
		}
		break;
	case MorphType::Matrial:
		this->material_offsets.resize(this->offset_count);
		for (auto& item : this->material_offsets)
		{
			item.parse(stream, setting);
		}
		break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
		this->uv_offsets.resize(this->offset_count);
		for (auto& item : this->uv_offsets)
		{
			item.parse(stream, setting);
		}
		break;
	default:
		throw;
	}
}

std::size_t pmx::Morph::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->morph_name, setting->encoding);
	total += pmx::util::dumpString(stream, this->morph_english_name, setting->encoding);


	stream.write(static_cast<char*>(static_cast<void*>(&this->category)), sizeof(MorphCategory));
	total += sizeof(MorphCategory);

	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_type)), sizeof(MorphType));
	total += sizeof(MorphType);

	stream.write(static_cast<char*>(static_cast<void*>(&this->offset_count)), sizeof(int));
	total += sizeof(int);

	switch (this->morph_type)
	{
	case MorphType::Group:
		for (int i = 0; i < offset_count; i++)
		{
			total += group_offsets[i].dump(stream, setting);
		}
		break;
	case MorphType::Vertex:
		for (int i = 0; i < offset_count; i++)
		{
			total += vertex_offsets[i].dump(stream, setting);
		}
		break;
	case MorphType::Bone:
		for (int i = 0; i < offset_count; i++)
		{
			total += bone_offsets[i].dump(stream, setting);
		}
		break;
	case MorphType::Matrial:
		for (int i = 0; i < offset_count; i++)
		{
			total += material_offsets[i].dump(stream, setting);
		}
		break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
		for (int i = 0; i < offset_count; i++)
		{
			total += uv_offsets[i].dump(stream, setting);
		}
		break;
	default:
		throw;
	}
	return total;
}
