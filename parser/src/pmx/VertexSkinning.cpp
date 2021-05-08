#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/VertexSkinning.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::VertexSkinningBDEF1::VertexSkinningBDEF1() noexcept
	: bone_index(0)
{}

void pmx::VertexSkinningBDEF1::parse(std::istream& stream, Setting* setting)
{
	this->bone_index = pmx::util::parseIndex(stream, setting->bone_index_size);
}

std::size_t pmx::VertexSkinningBDEF1::dump(std::ostream& stream, Setting* setting)
{
	return pmx::util::dumpIndex(stream, this->bone_index, setting->bone_index_size);
}

pmx::VertexSkinningBDEF2::VertexSkinningBDEF2() noexcept
	: bone_index1(0)
	, bone_index2(0)
	, bone_weight(0.0f)
{}

void pmx::VertexSkinningBDEF2::parse(std::istream& stream, Setting* setting)
{
	this->bone_index1 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight, sizeof(float));
}

std::size_t pmx::VertexSkinningBDEF2::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::VertexSkinningBDEF4::VertexSkinningBDEF4() noexcept
	: bone_index1(0)
	, bone_index2(0)
	, bone_index3(0)
	, bone_index4(0)
	, bone_weight1(0.0f)
	, bone_weight2(0.0f)
	, bone_weight3(0.0f)
	, bone_weight4(0.0f)
{}

void pmx::VertexSkinningBDEF4::parse(std::istream& stream, Setting* setting)
{
	this->bone_index1 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index3 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index4 = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight1, sizeof(float));
	stream.read((char*)&this->bone_weight2, sizeof(float));
	stream.read((char*)&this->bone_weight3, sizeof(float));
	stream.read((char*)&this->bone_weight4, sizeof(float));
}

std::size_t pmx::VertexSkinningBDEF4::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index3, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index4, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float) * 4;
	return total;
}

pmx::VertexSkinningSDEF::VertexSkinningSDEF() noexcept : bone_index1(0), bone_index2(0), bone_weight(0.0f)
{
	for (int i = 0; i < 3; ++i) {
		sdef_c[i] = 0.0f;
		sdef_r0[i] = 0.0f;
		sdef_r1[i] = 0.0f;
	}
}

void pmx::VertexSkinningSDEF::parse(std::istream& stream, Setting* setting)
{
	this->bone_index1 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight, sizeof(float));
	stream.read((char*)this->sdef_c, sizeof(float) * 3);
	stream.read((char*)this->sdef_r0, sizeof(float) * 3);
	stream.read((char*)this->sdef_r1, sizeof(float) * 3);
}

std::size_t pmx::VertexSkinningSDEF::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_c)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r0)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r1)), sizeof(float) * 3);
	total += sizeof(float) * 13;
	return total;
}

pmx::VertexSkinningQDEF::VertexSkinningQDEF() noexcept
	: bone_index1(0)
	, bone_index2(0)
	, bone_index3(0)
	, bone_index4(0)
	, bone_weight1(0.0f)
	, bone_weight2(0.0f)
	, bone_weight3(0.0f)
	, bone_weight4(0.0f)
{}

void pmx::VertexSkinningQDEF::parse(std::istream& stream, Setting* setting)
{
	this->bone_index1 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index3 = pmx::util::parseIndex(stream, setting->bone_index_size);
	this->bone_index4 = pmx::util::parseIndex(stream, setting->bone_index_size);
	stream.read((char*)&this->bone_weight1, sizeof(float));
	stream.read((char*)&this->bone_weight2, sizeof(float));
	stream.read((char*)&this->bone_weight3, sizeof(float));
	stream.read((char*)&this->bone_weight4, sizeof(float));
}

std::size_t pmx::VertexSkinningQDEF::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index3, setting->bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index4, setting->bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float) * 4;
	return total;
}
