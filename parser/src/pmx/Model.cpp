#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Model.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::Model::Model() noexcept
	: version(0.0f)
	, vertex_count(0)
	, index_count(0)
	, texture_count(0)
	, material_count(0)
	, bone_count(0)
	, morph_count(0)
	, frame_count(0)
	, rigid_body_count(0)
	, joint_count(0)
	, soft_body_count(0)
{}

void pmx::Model::Init()
{
	this->version = 0.0f;
	this->model_name.clear();
	this->model_english_name.clear();
	this->model_comment.clear();
	this->model_english_comment.clear();
	this->vertex_count = 0;
	this->vertices.clear();
	this->index_count = 0;
	this->indices.clear();
	this->texture_count = 0;
	this->textures.clear();
	this->material_count = 0;
	this->materials.clear();
	this->bone_count = 0;
	this->bones.clear();
	this->morph_count = 0;
	this->morphs.clear();
	this->frame_count = 0;
	this->frames.clear();
	this->rigid_body_count = 0;
	this->rigid_bodies.clear();
	this->joint_count = 0;
	this->joints.clear();
	this->soft_body_count = 0;
	this->soft_bodies.clear();
}

void pmx::Model::parse(std::istream& stream)
{
	// マジック
	char magic[4]{};
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
	this->model_name = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_english_name = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_comment = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_english_comment = std::move(pmx::util::parseString(stream, setting.encoding));

	// 頂点
	stream.read((char*)&vertex_count, sizeof(int));
	this->vertices.resize(vertex_count);
	for (auto& item : this->vertices)
	{
		item.parse(stream, &setting);
	}

	// 面
	stream.read((char*)&index_count, sizeof(int));
	this->indices.resize(index_count);
	for (auto& item : this->indices)
	{
		item = pmx::util::parseIndex(stream, setting.vertex_index_size);
	}

	// テクスチャ
	stream.read((char*)&texture_count, sizeof(int));
	this->textures.resize(texture_count);
	for (auto& item : this->textures)
	{
		item = pmx::util::parseString(stream, setting.encoding);
	}

	// マテリアル
	stream.read((char*)&material_count, sizeof(int));
	this->materials.resize(material_count);
	for(auto& item : this->materials)
	{
		item.parse(stream, &setting);
	}

	// ボーン
	stream.read((char*)&this->bone_count, sizeof(int));
	this->bones.resize(this->bone_count);
	for (auto& item : this->bones)
	{
		item.parse(stream, &setting);
	}

	// モーフ
	stream.read((char*)&this->morph_count, sizeof(int));
	this->morphs.resize(this->morph_count);
	for (auto& item : this->morphs)
	{
		item.parse(stream, &setting);
	}

	// 表示枠
	stream.read((char*)&this->frame_count, sizeof(int));
	this->frames.resize(this->frame_count);
	for (auto& item : this->frames)
	{
		item.parse(stream, &setting);
	}

	// 剛体
	stream.read((char*)&this->rigid_body_count, sizeof(int));
	this->rigid_bodies.resize(this->rigid_body_count);
	for (auto& item : this->rigid_bodies)
	{
		item.parse(stream, &setting);
	}

	// ジョイント
	stream.read((char*)&this->joint_count, sizeof(int));
	this->joints.resize(this->joint_count);
	for (auto& item : this->joints)
	{
		item.parse(stream, &setting);
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

std::size_t pmx::Model::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	// マジック
	constexpr char magic[4] = { 0x50, 0x4d, 0x58, 0x20 };
	stream.write(magic, sizeof(char) * 4);
	total += sizeof(char) * 4;


	// バージョン
	stream.write(static_cast<char*>(static_cast<void*>(&this->version)), sizeof(float));
	total += sizeof(float);

	// ファイル設定
	total += this->setting.dump(stream);

	// モデル情報
	total += pmx::util::dumpString(stream, this->model_name, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_english_name, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_comment, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_english_comment, setting.encoding);

	// 頂点
	stream.write(static_cast<char*>(static_cast<void*>(&this->vertex_count)), sizeof(int));
	total += sizeof(int);

	for(auto& item : this->vertices)
	{
		total += item.dump(stream, &setting);
	}

	// 面
	stream.write(static_cast<char*>(static_cast<void*>(&this->index_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->indices)
	{
		total += pmx::util::dumpIndex(stream, item, setting.vertex_index_size);
	}

	// テクスチャ
	stream.write(static_cast<char*>(static_cast<void*>(&this->texture_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->textures)
	{
		total += pmx::util::dumpString(stream, item, setting.encoding);
	}

	// マテリアル
	stream.write(static_cast<char*>(static_cast<void*>(&this->material_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->materials)
	{
		total += item.dump(stream, &setting);
	}

	// ボーン
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->bones)
	{
		total += item.dump(stream, &setting);
	}

	// モーフ
	stream.write(static_cast<char*>(static_cast<void*>(&this->morph_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->morphs)
	{
		total += item.dump(stream, &setting);
	}

	// 表示枠
	stream.write(static_cast<char*>(static_cast<void*>(&this->frame_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->frames)
	{
		total += item.dump(stream, &setting);
	}

	// 剛体
	stream.write(static_cast<char*>(static_cast<void*>(&this->rigid_body_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->rigid_bodies)
	{
		total += item.dump(stream, &setting);
	}

	// ジョイント
	stream.write(static_cast<char*>(static_cast<void*>(&this->joint_count)), sizeof(int));
	total += sizeof(int);

	for (auto& item : this->joints)
	{
		total += item.dump(stream, &setting);
	}
	return total;
}
