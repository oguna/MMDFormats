#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Vertex.h"

#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/util.h"

pmx::Vertex::Vertex() noexcept
: edge(0.0f)
{
	uv[0] = uv[1] = 0.0f;
	for (int i = 0; i < 3; ++i) {
		positon[i] = 0.0f;
		normal[i] = 0.0f;
	}
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			uva[i][k] = 0.0f;
		}
	}
}

void pmx::Vertex::parse(std::istream& stream, Setting* setting)
{
	stream.read((char*)this->positon, sizeof(float) * 3);
	stream.read((char*)this->normal, sizeof(float) * 3);
	stream.read((char*)this->uv, sizeof(float) * 2);
	for (int i = 0; i < setting->uv; ++i)
	{
		stream.read((char*)this->uva[i], sizeof(float) * 4);
	}
	stream.read((char*)&this->skinning_type, sizeof(pmx::VertexSkinningType));
	switch (this->skinning_type)
	{
	case pmx::VertexSkinningType::BDEF1:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF1>();
		this->skinning->parse(stream, setting);
		break;
	case pmx::VertexSkinningType::BDEF2:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF2>();
		this->skinning->parse(stream, setting);
		break;
	case pmx::VertexSkinningType::BDEF4:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF4>();
		this->skinning->parse(stream, setting);
		break;
	case pmx::VertexSkinningType::SDEF:
		this->skinning = std::make_unique<pmx::VertexSkinningSDEF>();
		this->skinning->parse(stream, setting);
		break;
	case pmx::VertexSkinningType::QDEF:
		this->skinning = std::make_unique<pmx::VertexSkinningQDEF>();
		this->skinning->parse(stream, setting);
		break;
	default:
		throw "invalid skinning type";
	}
	//this->skinning->parse(stream, setting);
	stream.read((char*)&this->edge, sizeof(float));
}

std::size_t pmx::Vertex::dump(std::ostream& stream, Setting* setting)
{
	std::size_t total{ 0 };
	stream.write(static_cast<char*>(static_cast<void*>(this->positon)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->normal)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->uv)), sizeof(float) * 2);
	total += sizeof(float) * 8;
	for (uint8_t i = 0; i < setting->uv; ++i)
	{
		stream.write(static_cast<char*>(static_cast<void*>(this->uva[i])), sizeof(float) * 4);
		total += sizeof(float) * 4;
	}
	stream.write(static_cast<char*>(static_cast<void*>(&this->skinning_type)), sizeof(pmx::VertexSkinningType));
	total += sizeof(pmx::VertexSkinningType);
	total += this->skinning->dump(stream, setting);
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge)), sizeof(float));
	total += sizeof(float);
	return total;
}
