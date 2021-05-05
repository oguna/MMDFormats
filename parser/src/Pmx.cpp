#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/Pmx.h"
#include "javacommons/strconv.h"

namespace {
	/// インデックス値を読み込む
	int parseIndex(std::istream& stream, int size)
	{
		switch (size)
		{
		case 1:
			uint8_t tmp8;
			stream.read((char*)&tmp8, sizeof(uint8_t));
			if (255 == tmp8)
			{
				return -1;
			}
			else {
				return (int)tmp8;
			}
		case 2:
			uint16_t tmp16;
			stream.read((char*)&tmp16, sizeof(uint16_t));
			if (65535 == tmp16)
			{
				return -1;
			}
			else {
				return (int)tmp16;
			}
		case 4:
			int tmp32;
			stream.read((char*)&tmp32, sizeof(int));
			return tmp32;
		default:
			return -1;
		}
	}

	size_t dumpIndex(std::ostream& stream, int index, int size)
	{
		switch (size)
		{
		case 1:
			stream.write(static_cast<char*>(static_cast<void*>(&index)), sizeof(uint8_t));
			return stream.good() ? sizeof(uint8_t) : 0;
		case 2:
			stream.write(static_cast<char*>(static_cast<void*>(&index)), sizeof(uint16_t));
			return stream.good() ? sizeof(uint16_t) : 0;
		case 4:
			stream.write(static_cast<char*>(static_cast<void*>(&index)), sizeof(int));
			return stream.good() ? sizeof(int) : 0;
		default:
			throw std::runtime_error("No such index size");
		}
	}

	/// 文字列を読み込む
	std::string parseString(std::istream& stream, uint8_t encoding)
	{
		int size;
		stream.read((char*)&size, sizeof(int));
		std::vector<char> buffer;
		if (size == 0)
		{
			return std::string("");
		}
		buffer.reserve(size);
		stream.read((char*)buffer.data(), size);
		if (encoding == 0)
		{
			// UTF16
			std::wstring tmpwstr(
				static_cast<wchar_t*>(
					static_cast<void*>(buffer.data())
					), size / 2);
			return wide_to_utf8(tmpwstr);
		}
		else
		{
			// UTF8
			return std::string(buffer.data(), size);
		}
	}

	size_t dumpString(std::ostream& stream, const std::string& str, uint8_t encoding)
	{
		int total{ 0 };
		std::vector<uint8_t> buf{};

		if (encoding == 0)
		{
			// UTF16
			std::wstring utf16str = utf8_to_wide(str);

			total = utf16str.length() *(sizeof(wchar_t) / sizeof(uint8_t));
			buf.resize(total);
			std::memcpy(buf.data(), utf16str.data(), total);
		}
		else
		{
			total = str.length();
			buf.resize(total);
			std::memcpy(buf.data(), str.data(), total);
		}
		stream.write(static_cast<char*>(static_cast<void*>(&total)), sizeof(int));
		stream.write(static_cast<char*>(static_cast<void*>(buf.data())), buf.size());
		return total+ sizeof(int);
	}
}

void pmx::PmxSetting::parse(std::istream& stream)
{
	uint8_t count;
	stream.read((char*)&count, sizeof(uint8_t));
	if (count < 8)
	{
		throw std::runtime_error("PMX Header is too short!");
	}
	stream.read((char*)&encoding, sizeof(uint8_t));
	stream.read((char*)&uv, sizeof(uint8_t));
	stream.read((char*)&vertex_index_size, sizeof(uint8_t));
	stream.read((char*)&texture_index_size, sizeof(uint8_t));
	stream.read((char*)&material_index_size, sizeof(uint8_t));
	stream.read((char*)&bone_index_size, sizeof(uint8_t));
	stream.read((char*)&morph_index_size, sizeof(uint8_t));
	stream.read((char*)&rigidbody_index_size, sizeof(uint8_t));
	uint8_t temp;
	for (int i = 8; i < count; i++)
	{
		stream.read((char*)&temp, sizeof(uint8_t));
	}
}

std::size_t pmx::PmxSetting::dump(std::ostream& stream)
{
	stream.write(u8"\x08", sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&encoding)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&uv)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&vertex_index_size)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&texture_index_size)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&material_index_size)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&bone_index_size)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&morph_index_size)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&rigidbody_index_size)), sizeof(uint8_t));
	return 9u;
}

void pmx::PmxVertexSkinningBDEF1::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index = parseIndex(stream, setting->bone_index_size);
}

std::size_t pmx::PmxVertexSkinningBDEF1::dump(std::ostream& stream, PmxSetting* setting)
{
	return dumpIndex(stream, this->bone_index, setting->bone_index_size);
}

void pmx::PmxVertexSkinningBDEF2::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index1 = parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight, sizeof(float));
}

std::size_t pmx::PmxVertexSkinningBDEF2::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

void pmx::PmxVertexSkinningBDEF4::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index1 = parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = parseIndex(stream, setting->bone_index_size);
	this->bone_index3 = parseIndex(stream, setting->bone_index_size);
	this->bone_index4 = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight1, sizeof(float));
	stream.read((char*)&this->bone_weight2, sizeof(float));
	stream.read((char*)&this->bone_weight3, sizeof(float));
	stream.read((char*)&this->bone_weight4, sizeof(float));
}

std::size_t pmx::PmxVertexSkinningBDEF4::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index3, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index4, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float)*4;
	return total;
}

void pmx::PmxVertexSkinningSDEF::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index1 = parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight, sizeof(float));
	stream.read((char*)this->sdef_c, sizeof(float) * 3);
	stream.read((char*)this->sdef_r0, sizeof(float) * 3);
	stream.read((char*)this->sdef_r1, sizeof(float) * 3);
}

std::size_t pmx::PmxVertexSkinningSDEF::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_c)), sizeof(float)*3);
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r0)), sizeof(float)*3);
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r1)), sizeof(float)*3);
	total += sizeof(float) * 12;
	return total;
}

void pmx::PmxVertexSkinningQDEF::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index1 = parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = parseIndex(stream, setting->bone_index_size);
	this->bone_index3 = parseIndex(stream, setting->bone_index_size);
	this->bone_index4 = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight1, sizeof(float));
	stream.read((char*)&this->bone_weight2, sizeof(float));
	stream.read((char*)&this->bone_weight3, sizeof(float));
	stream.read((char*)&this->bone_weight4, sizeof(float));
}

std::size_t pmx::PmxVertexSkinningQDEF::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index3, setting->bone_index_size);
	total += dumpIndex(stream, this->bone_index4, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float) * 4;
	return total;
}

void pmx::PmxVertex::parse(std::istream& stream, PmxSetting* setting)
{
	stream.read((char*)this->positon, sizeof(float) * 3);
	stream.read((char*)this->normal, sizeof(float) * 3);
	stream.read((char*)this->uv, sizeof(float) * 2);
	for (int i = 0; i < setting->uv; ++i)
	{
		stream.read((char*)this->uva[i], sizeof(float) * 4);
	}
	stream.read((char*)&this->skinning_type, sizeof(PmxVertexSkinningType));
	switch (this->skinning_type)
	{
	case PmxVertexSkinningType::BDEF1:
		this->skinning = std::make_unique<pmx::PmxVertexSkinningBDEF1>();
		this->skinning->parse(stream, setting);
		break;
	case PmxVertexSkinningType::BDEF2:
		this->skinning = std::make_unique<pmx::PmxVertexSkinningBDEF2>();
		this->skinning->parse(stream, setting);
		break;
	case PmxVertexSkinningType::BDEF4:
		this->skinning = std::make_unique<pmx::PmxVertexSkinningBDEF4>();
		this->skinning->parse(stream, setting);
		break;
	case PmxVertexSkinningType::SDEF:
		this->skinning = std::make_unique<pmx::PmxVertexSkinningSDEF>();
		this->skinning->parse(stream, setting);
		break;
	case PmxVertexSkinningType::QDEF:
		this->skinning = std::make_unique<pmx::PmxVertexSkinningQDEF>();
		this->skinning->parse(stream, setting);
		break;
	default:
		throw "invalid skinning type";
	}
	//this->skinning->parse(stream, setting);
	stream.read((char*)&this->edge, sizeof(float));
}

std::size_t pmx::PmxVertex::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	stream.write(static_cast<char*>(static_cast<void*>(this->positon)), sizeof(float)*3);
	stream.write(static_cast<char*>(static_cast<void*>(this->normal)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->uv)), sizeof(float) * 2);
	total += sizeof(float) * 8;
	for (uint8_t i = 0; i < setting->uv; ++i)
	{
		stream.write(static_cast<char*>(static_cast<void*>(this->uva[i])), sizeof(float) * 4);
		total += sizeof(float) * 4;
	}
	stream.write(static_cast<char*>(static_cast<void*>(&this->skinning_type)), sizeof(PmxVertexSkinningType));
	total += sizeof(PmxVertexSkinningType);
	total += this->skinning->dump(stream, setting);
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge)), sizeof(float));
	total += sizeof(float);
	return total;
}

void pmx::PmxMaterial::parse(std::istream& stream, PmxSetting* setting)
{
	this->material_name = std::move(parseString(stream, setting->encoding));
	this->material_english_name = std::move(parseString(stream, setting->encoding));
	stream.read((char*)this->diffuse, sizeof(float) * 4);
	stream.read((char*)this->specular, sizeof(float) * 3);
	stream.read((char*)&this->specularlity, sizeof(float));
	stream.read((char*)this->ambient, sizeof(float) * 3);
	stream.read((char*)&this->flag, sizeof(uint8_t));
	stream.read((char*)this->edge_color, sizeof(float) * 4);
	stream.read((char*)&this->edge_size, sizeof(float));
	this->diffuse_texture_index = parseIndex(stream, setting->texture_index_size);
	this->sphere_texture_index = parseIndex(stream, setting->texture_index_size);
	stream.read((char*)&this->sphere_op_mode, sizeof(uint8_t));
	stream.read((char*)&this->common_toon_flag, sizeof(uint8_t));
	if (this->common_toon_flag)
	{
		stream.read((char*)&this->toon_texture_index, sizeof(uint8_t));
	}
	else {
		this->toon_texture_index = parseIndex(stream, setting->texture_index_size);
	}
	this->memo = std::move(parseString(stream, setting->encoding));
	stream.read((char*)&this->index_count, sizeof(int));
}

std::size_t pmx::PmxMaterial::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, this->material_name, setting->encoding);
	total += dumpString(stream, this->material_english_name, setting->encoding);
	stream.write(static_cast<char*>(static_cast<void*>(this->diffuse)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(this->specular)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->specularlity)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(this->ambient)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->flag)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(this->edge_color)), sizeof(float) * 4);
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge_size)), sizeof(float));
	total += sizeof(float) * 16 + sizeof(uint8_t);
	total += dumpIndex(stream, this->diffuse_texture_index, setting->texture_index_size);
	total += dumpIndex(stream, this->sphere_texture_index, setting->texture_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->sphere_op_mode)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->common_toon_flag)), sizeof(uint8_t));
	total += sizeof(uint8_t) * 2;
	if (this->common_toon_flag)
	{
		stream.write(static_cast<char*>(static_cast<void*>(&this->toon_texture_index)), sizeof(uint8_t));
		total += sizeof(uint8_t);
	}
	else {
		total += dumpIndex(stream, this->toon_texture_index, setting->texture_index_size);
	}
	total += dumpString(stream, this->memo, setting->encoding);
	stream.write(static_cast<char*>(static_cast<void*>(&this->index_count)), sizeof(int));
	total += sizeof(int);
	return total;
}

void pmx::PmxIkLink::parse(std::istream& stream, PmxSetting* setting)
{
	this->link_target = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->angle_lock, sizeof(uint8_t));
	if (angle_lock == 1)
	{
		stream.read((char*)this->max_radian, sizeof(float) * 3);
		stream.read((char*)this->min_radian, sizeof(float) * 3);
	}
}

std::size_t pmx::PmxIkLink::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->link_target, setting->bone_index_size);
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

void pmx::PmxBone::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_name = std::move(parseString(stream, setting->encoding));
	this->bone_english_name = std::move(parseString(stream, setting->encoding));
	stream.read((char*)this->position, sizeof(float) * 3);
	this->parent_index = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->level, sizeof(int));
	stream.read((char*)&this->bone_flag, sizeof(uint16_t));
	if (this->bone_flag & 0x0001) {
		this->target_index = parseIndex(stream, setting->bone_index_size);
	}
	else {
		stream.read((char*)this->offset, sizeof(float) * 3);
	}
	if (this->bone_flag & (0x0100 | 0x0200)) {
		this->grant_parent_index = parseIndex(stream, setting->bone_index_size);
		stream.read((char*)&this->grant_weight, sizeof(float));
	}
	if (this->bone_flag & 0x0400) {
		stream.read((char*)this->lock_axis_orientation, sizeof(float) * 3);
	}
	if (this->bone_flag & 0x0800) {
		stream.read((char*)this->local_axis_x_orientation, sizeof(float) * 3);
		stream.read((char*)this->local_axis_y_orientation, sizeof(float) * 3);
	}
	if (this->bone_flag & 0x2000) {
		stream.read((char*)&this->key, sizeof(int));
	}
	if (this->bone_flag & 0x0020) {
		this->ik_target_bone_index = parseIndex(stream, setting->bone_index_size);
		stream.read((char*)&ik_loop, sizeof(int));
		stream.read((char*)&ik_loop_angle_limit, sizeof(float));
		stream.read((char*)&ik_link_count, sizeof(int));
		this->ik_links = std::make_unique<PmxIkLink[]>(ik_link_count);
		for (int i = 0; i < ik_link_count; i++) {
			ik_links[i].parse(stream, setting);
		}
	}
}

std::size_t pmx::PmxBone::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, this->bone_name, setting->encoding);
	total += dumpString(stream, this->bone_english_name, setting->encoding);

	stream.write(static_cast<char*>(static_cast<void*>(&this->position)), sizeof(float) * 3);
	total += sizeof(float) * 3;

	total += dumpIndex(stream, this->parent_index, setting->bone_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(&this->level)), sizeof(int));
	total += sizeof(int);

	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_flag)), sizeof(uint16_t));
	total += sizeof(uint16_t);

	if (this->bone_flag & 0x0001) {
		total += dumpIndex(stream, this->target_index, setting->bone_index_size);
	}
	else {
		stream.write(static_cast<char*>(static_cast<void*>(&this->offset)), sizeof(float) * 3);
		total += sizeof(float) * 3;
	}
	if (this->bone_flag & (0x0100 | 0x0200)) {
		total += dumpIndex(stream, this->grant_parent_index, setting->bone_index_size);
		stream.write(static_cast<char*>(static_cast<void*>(&this->grant_weight)), sizeof(float));
		total += sizeof(float);
	}
	if (this->bone_flag & 0x0400) {
		stream.write(static_cast<char*>(static_cast<void*>(&this->lock_axis_orientation)), sizeof(float) * 3);
		total += sizeof(float) * 3;
	}
	if (this->bone_flag & 0x0800) {
		stream.write(static_cast<char*>(static_cast<void*>(&this->local_axis_x_orientation)), sizeof(float) * 3);
		stream.write(static_cast<char*>(static_cast<void*>(&this->local_axis_y_orientation)), sizeof(float) * 3);
		total += sizeof(float) * 6;
	}
	if (this->bone_flag & 0x2000) {
		stream.write(static_cast<char*>(static_cast<void*>(&this->key)), sizeof(int));
		total += sizeof(int);
	}
	if (this->bone_flag & 0x0020) {
		total += dumpIndex(stream, this->ik_target_bone_index, setting->bone_index_size);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_loop)), sizeof(int));
		total += sizeof(int);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_loop_angle_limit)), sizeof(float));
		total += sizeof(float);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_link_count)), sizeof(int));
		total += sizeof(int);

		for (int i = 0; i < this->ik_link_count; i++) {
			total += ik_links[i].dump(stream, setting);
		}
	}

	return total;
}

void pmx::PmxMorphVertexOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->vertex_index = parseIndex(stream, setting->vertex_index_size);
	stream.read((char*)this->position_offset, sizeof(float) * 3);
}

std::size_t pmx::PmxMorphVertexOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->vertex_index, setting->vertex_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->position_offset)), sizeof(float) * 3);
	total += sizeof(float) * 3;
	return total;
}

void pmx::PmxMorphUVOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->vertex_index = parseIndex(stream, setting->vertex_index_size);
	stream.read((char*)this->uv_offset, sizeof(float) * 4);
}

std::size_t pmx::PmxMorphUVOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->vertex_index, setting->vertex_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->uv_offset)), sizeof(float) * 4);
	total += sizeof(float) * 4;
	return total;
}

void pmx::PmxMorphBoneOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->bone_index = parseIndex(stream, setting->bone_index_size);
	stream.read((char*)this->translation, sizeof(float) * 3);
	stream.read((char*)this->rotation, sizeof(float) * 4);
}

std::size_t pmx::PmxMorphBoneOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->bone_index, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->translation)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->rotation)), sizeof(float) * 4);
	total += sizeof(float) * 7;
	return total;
}

void pmx::PmxMorphMaterialOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->material_index = parseIndex(stream, setting->material_index_size);
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

std::size_t pmx::PmxMorphMaterialOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->material_index, setting->material_index_size);
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

void pmx::PmxMorphGroupOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->morph_index = parseIndex(stream, setting->morph_index_size);
	stream.read((char*)&this->morph_weight, sizeof(float));
}

std::size_t pmx::PmxMorphGroupOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->morph_index, setting->morph_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

void pmx::PmxMorphFlipOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->morph_index = parseIndex(stream, setting->morph_index_size);
	stream.read((char*)&this->morph_value, sizeof(float));
}

std::size_t pmx::PmxMorphFlipOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->morph_index, setting->morph_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_value)), sizeof(float));
	total += sizeof(float);
	return total;
}

void pmx::PmxMorphImpulseOffset::parse(std::istream& stream, PmxSetting* setting)
{
	this->rigid_body_index = parseIndex(stream, setting->rigidbody_index_size);
	stream.read((char*)&this->is_local, sizeof(uint8_t));
	stream.read((char*)this->velocity, sizeof(float) * 3);
	stream.read((char*)this->angular_torque, sizeof(float) * 3);
}

std::size_t pmx::PmxMorphImpulseOffset::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->rigid_body_index, setting->rigidbody_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->is_local)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->velocity)), sizeof(float)*3);
	stream.write(static_cast<char*>(static_cast<void*>(&this->angular_torque)), sizeof(float)*3);
	total += sizeof(float) * 6 + sizeof(uint8_t);
	return total;
}

void pmx::PmxMorph::parse(std::istream& stream, PmxSetting* setting)
{
	this->morph_name = parseString(stream, setting->encoding);
	this->morph_english_name = parseString(stream, setting->encoding);
	stream.read((char*)&category, sizeof(MorphCategory));
	stream.read((char*)&morph_type, sizeof(MorphType));
	stream.read((char*)&this->offset_count, sizeof(int));
	switch (this->morph_type)
	{
	case MorphType::Group:
		group_offsets = std::make_unique<PmxMorphGroupOffset[]>(this->offset_count);
		for (int i = 0; i < offset_count; i++)
		{
			group_offsets[i].parse(stream, setting);
		}
		break;
	case MorphType::Vertex:
		vertex_offsets = std::make_unique<PmxMorphVertexOffset[]>(this->offset_count);
		for (int i = 0; i < offset_count; i++)
		{
			vertex_offsets[i].parse(stream, setting);
		}
		break;
	case MorphType::Bone:
		bone_offsets = std::make_unique<PmxMorphBoneOffset[]>(this->offset_count);
		for (int i = 0; i < offset_count; i++)
		{
			bone_offsets[i].parse(stream, setting);
		}
		break;
	case MorphType::Matrial:
		material_offsets = std::make_unique<PmxMorphMaterialOffset[]>(this->offset_count);
		for (int i = 0; i < offset_count; i++)
		{
			material_offsets[i].parse(stream, setting);
		}
		break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
		uv_offsets = std::make_unique<PmxMorphUVOffset[]>(this->offset_count);
		for (int i = 0; i < offset_count; i++)
		{
			uv_offsets[i].parse(stream, setting);
		}
		break;
	default:
		throw;
	}
}

std::size_t pmx::PmxMorph::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, this->morph_name, setting->encoding);
	total += dumpString(stream, this->morph_english_name, setting->encoding);


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

void pmx::PmxFrameElement::parse(std::istream& stream, PmxSetting* setting)
{
	stream.read((char*)&this->element_target, sizeof(uint8_t));
	if (this->element_target == 0x00)
	{
		this->index = parseIndex(stream, setting->bone_index_size);
	}
	else {
		this->index = parseIndex(stream, setting->morph_index_size);
	}
}

std::size_t pmx::PmxFrameElement::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	stream.write(static_cast<char*>(static_cast<void*>(&this->element_target)), sizeof(uint8_t));
	total += sizeof(uint8_t);
	if (this->element_target == 0x00)
	{
		total += dumpIndex(stream, this->index, setting->bone_index_size);
	}
	else {
		total += dumpIndex(stream, this->index, setting->morph_index_size);
	}
	return total;
}

void pmx::PmxFrame::parse(std::istream& stream, PmxSetting* setting)
{
	this->frame_name = parseString(stream, setting->encoding);
	this->frame_english_name = parseString(stream, setting->encoding);
	stream.read((char*)&this->frame_flag, sizeof(uint8_t));
	stream.read((char*)&this->element_count, sizeof(int));
	this->elements = std::make_unique<PmxFrameElement[]>(this->element_count);
	for (int i = 0; i < this->element_count; i++)
	{
		this->elements[i].parse(stream, setting);
	}
}

std::size_t pmx::PmxFrame::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, this->frame_name, setting->encoding);
	total += dumpString(stream, this->frame_english_name, setting->encoding);


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

void pmx::PmxRigidBody::parse(std::istream& stream, PmxSetting* setting)
{
	this->rigid_body_name = parseString(stream, setting->encoding);
	this->rigid_body_english_name = parseString(stream, setting->encoding);
	this->target_bone = parseIndex(stream, setting->bone_index_size);
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

std::size_t pmx::PmxRigidBody::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, this->rigid_body_name, setting->encoding);
	total += dumpString(stream, this->rigid_body_english_name, setting->encoding);
	total += dumpIndex(stream, this->target_bone, setting->bone_index_size);

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
	
	total += sizeof(float)*14 + sizeof(uint16_t) + sizeof(uint8_t)*3;
	return total;
}

void pmx::PmxJointParam::parse(std::istream& stream, PmxSetting* setting)
{
	this->rigid_body1 = parseIndex(stream, setting->rigidbody_index_size);
	this->rigid_body2 = parseIndex(stream, setting->rigidbody_index_size);
	stream.read((char*)this->position, sizeof(float) * 3);
	stream.read((char*)this->orientaiton, sizeof(float) * 3);
	stream.read((char*)this->move_limitation_min, sizeof(float) * 3);
	stream.read((char*)this->move_limitation_max, sizeof(float) * 3);
	stream.read((char*)this->rotation_limitation_min, sizeof(float) * 3);
	stream.read((char*)this->rotation_limitation_max, sizeof(float) * 3);
	stream.read((char*)this->spring_move_coefficient, sizeof(float) * 3);
	stream.read((char*)this->spring_rotation_coefficient, sizeof(float) * 3);
}

std::size_t pmx::PmxJointParam::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->rigid_body1, setting->rigidbody_index_size);
	total += dumpIndex(stream, this->rigid_body2, setting->rigidbody_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(this->position)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->orientaiton)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_min)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_max)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_min)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_max)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_move_coefficient)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_rotation_coefficient)), sizeof(float) * 3);

	total += sizeof(float) * 24;
	return total;
}

void pmx::PmxJoint::parse(std::istream& stream, PmxSetting* setting)
{
	this->joint_name = parseString(stream, setting->encoding);
	this->joint_english_name = parseString(stream, setting->encoding);
	stream.read((char*)&this->joint_type, sizeof(uint8_t));
	this->param.parse(stream, setting);
}

std::size_t pmx::PmxJoint::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpString(stream, joint_name, setting->encoding);
	total += dumpString(stream, joint_english_name, setting->encoding);

	stream.write(static_cast<char*>(static_cast<void*>(&this->joint_type)), sizeof(uint8_t));
	total += sizeof(uint8_t);

	total += this->param.dump(stream, setting);
	return total;
}

void pmx::PmxAnchorRigidBody::parse(std::istream& stream, PmxSetting* setting)
{
	this->related_rigid_body = parseIndex(stream, setting->rigidbody_index_size);
	this->related_vertex = parseIndex(stream, setting->vertex_index_size);
	stream.read((char*)&this->is_near, sizeof(uint8_t));
}

std::size_t pmx::PmxAnchorRigidBody::dump(std::ostream& stream, PmxSetting* setting)
{
	std::size_t total{ 0 };
	total += dumpIndex(stream, this->related_rigid_body, setting->rigidbody_index_size);
	total += dumpIndex(stream, this->related_vertex, setting->vertex_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(&this->is_near)), sizeof(uint8_t));
	total += sizeof(uint8_t);
	return total;
}

void pmx::PmxSoftBody::parse(std::istream& stream, PmxSetting* setting)
{
	// 未実装
	throw std::runtime_error("Not Implemented Exception");
}

std::size_t pmx::PmxSoftBody::dump(std::ostream& stream, PmxSetting* setting)
{
	throw std::runtime_error("Not Implemented Exception");
}

void pmx::PmxModel::Init()
{
	this->version = 0.0f;
	this->model_name.clear();
	this->model_english_name.clear();
	this->model_comment.clear();
	this->model_english_comment.clear();
	this->vertex_count = 0;
	this->vertices = nullptr;
	this->index_count = 0;
	this->indices = nullptr;
	this->texture_count = 0;
	this->textures = nullptr;
	this->material_count = 0;
	this->materials = nullptr;
	this->bone_count = 0;
	this->bones = nullptr;
	this->morph_count = 0;
	this->morphs = nullptr;
	this->frame_count = 0;
	this->frames = nullptr;
	this->rigid_body_count = 0;
	this->rigid_bodies = nullptr;
	this->joint_count = 0;
	this->joints = nullptr;
	this->soft_body_count = 0;
	this->soft_bodies = nullptr;
}

void pmx::PmxModel::parse(std::istream& stream)
{
	// マジック
	char magic[4];
	stream.read((char*)magic, sizeof(char) * 4);
	if (magic[0] != 0x50 || magic[1] != 0x4d || magic[2] != 0x58 || magic[3] != 0x20)
	{
		std::cerr << "invalid magic number." << std::endl;
		throw;
	}
	// バージョン
	stream.read((char*)&version, sizeof(float));
	if (version != 2.0f && version != 2.1f)
	{
		std::cerr << "this is not ver2.0 or ver2.1 but " << version << "." << std::endl;
		throw;
	}
	// ファイル設定
	this->setting.parse(stream);

	// モデル情報
	this->model_name = std::move(parseString(stream, setting.encoding));
	this->model_english_name = std::move(parseString(stream, setting.encoding));
	this->model_comment = std::move(parseString(stream, setting.encoding));
	this->model_english_comment = std::move(parseString(stream, setting.encoding));

	// 頂点
	stream.read((char*)&vertex_count, sizeof(int));
	this->vertices = std::make_unique<PmxVertex[]>(vertex_count);
	for (int i = 0; i < vertex_count; i++)
	{
		vertices[i].parse(stream, &setting);
	}

	// 面
	stream.read((char*)&index_count, sizeof(int));
	this->indices = std::make_unique<int[]>(index_count);
	for (int i = 0; i < index_count; i++)
	{
		this->indices[i] = parseIndex(stream, setting.vertex_index_size);
	}

	// テクスチャ
	stream.read((char*)&texture_count, sizeof(int));
	this->textures = std::make_unique<std::string[]>(texture_count);
	for (int i = 0; i < texture_count; i++)
	{
		this->textures[i] = parseString(stream, setting.encoding);
	}

	// マテリアル
	stream.read((char*)&material_count, sizeof(int));
	this->materials = std::make_unique<PmxMaterial[]>(material_count);
	for (int i = 0; i < material_count; i++)
	{
		this->materials[i].parse(stream, &setting);
	}

	// ボーン
	stream.read((char*)&this->bone_count, sizeof(int));
	this->bones = std::make_unique<PmxBone[]>(this->bone_count);
	for (int i = 0; i < this->bone_count; i++)
	{
		this->bones[i].parse(stream, &setting);
	}

	// モーフ
	stream.read((char*)&this->morph_count, sizeof(int));
	this->morphs = std::make_unique<PmxMorph[]>(this->morph_count);
	for (int i = 0; i < this->morph_count; i++)
	{
		this->morphs[i].parse(stream, &setting);
	}

	// 表示枠
	stream.read((char*)&this->frame_count, sizeof(int));
	this->frames = std::make_unique<PmxFrame[]>(this->frame_count);
	for (int i = 0; i < this->frame_count; i++)
	{
		this->frames[i].parse(stream, &setting);
	}

	// 剛体
	stream.read((char*)&this->rigid_body_count, sizeof(int));
	this->rigid_bodies = std::make_unique<PmxRigidBody[]>(this->rigid_body_count);
	for (int i = 0; i < this->rigid_body_count; i++)
	{
		this->rigid_bodies[i].parse(stream, &setting);
	}

	// ジョイント
	stream.read((char*)&this->joint_count, sizeof(int));
	this->joints = std::make_unique<PmxJoint[]>(this->joint_count);
	for (int i = 0; i < this->joint_count; i++)
	{
		this->joints[i].parse(stream, &setting);
	}

	//// ソフトボディ
	//if (this->version == 2.1f)
	//{
	//	stream.read((char*) &this->soft_body_count, sizeof(int));
	//	this->soft_bodies = std::make_unique<PmxSoftBody []>(this->soft_body_count);
	//	for (int i = 0; i < this->soft_body_count; i++)
	//	{
	//		this->soft_bodies[i].parse(stream, &setting);
	//	}
	//}
}

std::size_t pmx::PmxModel::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	// マジック
	constexpr char magic[4] = {0x50, 0x4d, 0x58, 0x20};
	stream.write(magic, sizeof(char) * 4);
	total += sizeof(char) * 4;


	// バージョン
	stream.write(static_cast<char*>(static_cast<void*>(&this->version)), sizeof(float));
	total += sizeof(float);

	// ファイル設定
	total += this->setting.dump(stream);

	// モデル情報
	total += dumpString(stream, this->model_name, setting.encoding);
	total += dumpString(stream, this->model_english_name, setting.encoding);
	total += dumpString(stream, this->model_comment, setting.encoding);
	total += dumpString(stream, this->model_english_comment, setting.encoding);
	
	// 頂点
	stream.write(static_cast<char*>(static_cast<void*>(&this->vertex_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < vertex_count; i++)
	{
		total += vertices[i].dump(stream, &setting);
	}

	// 面
	stream.write(static_cast<char*>(static_cast<void*>(&this->index_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < index_count; i++)
	{
		total += dumpIndex(stream, this->indices[i], setting.vertex_index_size);
	}

	// テクスチャ
	stream.write(static_cast<char*>(static_cast<void*>(&this->texture_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < texture_count; i++)
	{
		total += dumpString(stream, this->textures[i], setting.encoding);
	}

	// マテリアル
	stream.write(static_cast<char*>(static_cast<void*>(&this->material_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < material_count; i++)
	{
		total += this->materials[i].dump(stream, &setting);
	}

	// ボーン
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->bone_count; i++)
	{
		total += this->bones[i].dump(stream, &setting);
	}

	// モーフ
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->morph_count; i++)
	{
		total += this->morphs[i].dump(stream, &setting);
	}

	// 表示枠
	stream.write(static_cast<char*>(static_cast<void*>(&this->frame_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->frame_count; i++)
	{
		total += this->frames[i].dump(stream, &setting);
	}

	// 剛体
	stream.write(static_cast<char*>(static_cast<void*>(&this->rigid_body_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->rigid_body_count; i++)
	{
		total += this->rigid_bodies[i].dump(stream, &setting);
	}

	// ジョイント
	stream.write(static_cast<char*>(static_cast<void*>(&this->joint_count)), sizeof(int));
	total += sizeof(int);

	for (int i = 0; i < this->joint_count; i++)
	{
		total += this->joints[i].dump(stream, &setting);
	}
	return total;
}
