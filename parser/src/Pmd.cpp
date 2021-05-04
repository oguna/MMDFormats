#include "Pmd.h"

using namespace pmd;

bool pmd::PmdHeader::Read(std::ifstream * stream)
{
	char buffer[256];
	stream->read(buffer, 20);
	name = std::string(buffer);
	stream->read(buffer, 256);
	comment = std::string(buffer);
	return true;
}

bool pmd::PmdHeader::ReadExtension(std::ifstream * stream)
{
	char buffer[256];
	stream->read(buffer, 20);
	name_english = std::string(buffer);
	stream->read(buffer, 256);
	comment_english = std::string(buffer);
	return true;
}

bool pmd::PmdVertex::Read(std::ifstream * stream)
{
	stream->read((char*)position, sizeof(float) * 3);
	stream->read((char*)normal, sizeof(float) * 3);
	stream->read((char*)uv, sizeof(float) * 2);
	stream->read((char*)bone_index, sizeof(uint16_t) * 2);
	stream->read((char*)&bone_weight, sizeof(uint8_t));
	stream->read((char*)&edge_invisible, sizeof(uint8_t));
	return true;
}

bool pmd::PmdMaterial::Read(std::ifstream * stream)
{
	char buffer[20];
	stream->read((char*)&diffuse, sizeof(float) * 4);
	stream->read((char*)&power, sizeof(float));
	stream->read((char*)&specular, sizeof(float) * 3);
	stream->read((char*)&ambient, sizeof(float) * 3);
	stream->read((char*)&toon_index, sizeof(uint8_t));
	stream->read((char*)&edge_flag, sizeof(uint8_t));
	stream->read((char*)&index_count, sizeof(uint32_t));
	stream->read((char*)&buffer, sizeof(char) * 20);
	char* pstar = strchr(buffer, '*');
	if (NULL == pstar)
	{
		texture_filename = std::string(buffer);
		sphere_filename.clear();
	}
	else {
		*pstar = NULL;
		texture_filename = std::string(buffer);
		sphere_filename = std::string(pstar + 1);
	}
	return true;
}

void pmd::PmdBone::Read(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, 20);
	name = std::string(buffer);
	stream->read((char*)&parent_bone_index, sizeof(uint16_t));
	stream->read((char*)&tail_pos_bone_index, sizeof(uint16_t));
	stream->read((char*)&bone_type, sizeof(uint8_t));
	stream->read((char*)&ik_parent_bone_index, sizeof(uint16_t));
	stream->read((char*)&bone_head_pos, sizeof(float) * 3);
}

void pmd::PmdBone::ReadExpantion(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, 20);
	name_english = std::string(buffer);
}

void pmd::PmdIk::Read(std::istream * stream)
{
	stream->read((char *)&ik_bone_index, sizeof(uint16_t));
	stream->read((char *)&target_bone_index, sizeof(uint16_t));
	uint8_t ik_chain_length;
	stream->read((char*)&ik_chain_length, sizeof(uint8_t));
	stream->read((char *)&interations, sizeof(uint16_t));
	stream->read((char *)&angle_limit, sizeof(float));
	ik_child_bone_index.resize(ik_chain_length);
	for (int i = 0; i < ik_chain_length; i++)
	{
		stream->read((char *)&ik_child_bone_index[i], sizeof(uint16_t));
	}
}

void pmd::PmdFaceVertex::Read(std::istream * stream)
{
	stream->read((char *)&vertex_index, sizeof(int));
	stream->read((char *)position, sizeof(float) * 3);
}

void pmd::PmdFace::Read(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, 20);
	name = std::string(buffer);
	int vertex_count;
	stream->read((char*)&vertex_count, sizeof(int));
	stream->read((char*)&type, sizeof(uint8_t));
	vertices.resize(vertex_count);
	for (int i = 0; i < vertex_count; i++)
	{
		vertices[i].Read(stream);
	}
}

void pmd::PmdFace::ReadExpantion(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, 20);
	name_english = std::string(buffer);
}

void pmd::PmdBoneDispName::Read(std::istream * stream)
{
	char buffer[50];
	stream->read(buffer, 50);
	bone_disp_name = std::string(buffer);
	bone_disp_name_english.clear();
}

void pmd::PmdBoneDispName::ReadExpantion(std::istream * stream)
{
	char buffer[50];
	stream->read(buffer, 50);
	bone_disp_name_english = std::string(buffer);
}

void pmd::PmdBoneDisp::Read(std::istream * stream)
{
	stream->read((char*)&bone_index, sizeof(uint16_t));
	stream->read((char*)&bone_disp_index, sizeof(uint8_t));
}

void pmd::PmdRigidBody::Read(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, sizeof(char) * 20);
	name = (std::string(buffer));
	stream->read((char*)&related_bone_index, sizeof(uint16_t));
	stream->read((char*)&group_index, sizeof(uint8_t));
	stream->read((char*)&mask, sizeof(uint16_t));
	stream->read((char*)&shape, sizeof(uint8_t));
	stream->read((char*)size, sizeof(float) * 3);
	stream->read((char*)position, sizeof(float) * 3);
	stream->read((char*)orientation, sizeof(float) * 3);
	stream->read((char*)&weight, sizeof(float));
	stream->read((char*)&linear_damping, sizeof(float));
	stream->read((char*)&anglar_damping, sizeof(float));
	stream->read((char*)&restitution, sizeof(float));
	stream->read((char*)&friction, sizeof(float));
	stream->read((char*)&rigid_type, sizeof(char));
}

void pmd::PmdConstraint::Read(std::istream * stream)
{
	char buffer[20];
	stream->read(buffer, 20);
	name = std::string(buffer);
	stream->read((char *)&rigid_body_index_a, sizeof(uint32_t));
	stream->read((char *)&rigid_body_index_b, sizeof(uint32_t));
	stream->read((char *)position, sizeof(float) * 3);
	stream->read((char *)orientation, sizeof(float) * 3);
	stream->read((char *)linear_lower_limit, sizeof(float) * 3);
	stream->read((char *)linear_upper_limit, sizeof(float) * 3);
	stream->read((char *)angular_lower_limit, sizeof(float) * 3);
	stream->read((char *)angular_upper_limit, sizeof(float) * 3);
	stream->read((char *)linear_stiffness, sizeof(float) * 3);
	stream->read((char *)angular_stiffness, sizeof(float) * 3);
}

std::unique_ptr<PmdModel> pmd::PmdModel::LoadFromFile(const char * filename)
{
	std::ifstream stream(filename, std::ios::binary);
	if (stream.fail())
	{
		std::cerr << "could not open \"" << filename << "\"" << std::endl;
		return nullptr;
	}
	auto result = LoadFromStream(&stream);
	stream.close();
	return result;
}

/// ƒtƒ@ƒCƒ‹‚©‚çPmdModel‚ð¶¬‚·‚é

std::unique_ptr<PmdModel> pmd::PmdModel::LoadFromStream(std::ifstream * stream)
{
	auto result = std::make_unique<PmdModel>();
	char buffer[100];

	// magic
	char magic[3];
	stream->read(magic, 3);
	if (magic[0] != 'P' || magic[1] != 'm' || magic[2] != 'd')
	{
		std::cerr << "invalid file" << std::endl;
		return nullptr;
	}

	// version
	stream->read((char*) &(result->version), sizeof(float));
	if (result->version != 1.0f)
	{
		std::cerr << "invalid version" << std::endl;
		return nullptr;
	}

	// header
	result->header.Read(stream);

	// vertices
	uint32_t vertex_num;
	stream->read((char*)&vertex_num, sizeof(uint32_t));
	result->vertices.resize(vertex_num);
	for (uint32_t i = 0; i < vertex_num; i++)
	{
		result->vertices[i].Read(stream);
	}

	// indices
	uint32_t index_num;
	stream->read((char*)&index_num, sizeof(uint32_t));
	result->indices.resize(index_num);
	for (uint32_t i = 0; i < index_num; i++)
	{
		stream->read((char*)&result->indices[i], sizeof(uint16_t));
	}

	// materials
	uint32_t material_num;
	stream->read((char*)&material_num, sizeof(uint32_t));
	result->materials.resize(material_num);
	for (uint32_t i = 0; i < material_num; i++)
	{
		result->materials[i].Read(stream);
	}

	// bones
	uint16_t bone_num;
	stream->read((char*)&bone_num, sizeof(uint16_t));
	result->bones.resize(bone_num);
	for (uint32_t i = 0; i < bone_num; i++)
	{
		result->bones[i].Read(stream);
	}

	// iks
	uint16_t ik_num;
	stream->read((char*)&ik_num, sizeof(uint16_t));
	result->iks.resize(ik_num);
	for (uint32_t i = 0; i < ik_num; i++)
	{
		result->iks[i].Read(stream);
	}

	// faces
	uint16_t face_num;
	stream->read((char*)&face_num, sizeof(uint16_t));
	result->faces.resize(face_num);
	for (uint32_t i = 0; i < face_num; i++)
	{
		result->faces[i].Read(stream);
	}

	// face frames
	uint8_t face_frame_num;
	stream->read((char*)&face_frame_num, sizeof(uint8_t));
	result->faces_indices.resize(face_frame_num);
	for (uint32_t i = 0; i < face_frame_num; i++)
	{
		stream->read((char*)&result->faces_indices[i], sizeof(uint16_t));
	}

	// bone names
	uint8_t bone_disp_num;
	stream->read((char*)&bone_disp_num, sizeof(uint8_t));
	result->bone_disp_name.resize(bone_disp_num);
	for (uint32_t i = 0; i < bone_disp_num; i++)
	{
		result->bone_disp_name[i].Read(stream);
	}

	// bone frame
	uint32_t bone_frame_num;
	stream->read((char*)&bone_frame_num, sizeof(uint32_t));
	result->bone_disp.resize(bone_frame_num);
	for (uint32_t i = 0; i < bone_frame_num; i++)
	{
		result->bone_disp[i].Read(stream);
	}

	// english name
	bool english;
	stream->read((char*)&english, sizeof(char));
	if (english)
	{
		result->header.ReadExtension(stream);
		for (uint32_t i = 0; i < bone_num; i++)
		{
			result->bones[i].ReadExpantion(stream);
		}
		for (uint32_t i = 0; i < face_num; i++)
		{
			if (result->faces[i].type == pmd::FaceCategory::Base)
			{
				continue;
			}
			result->faces[i].ReadExpantion(stream);
		}
		for (uint32_t i = 0; i < result->bone_disp_name.size(); i++)
		{
			result->bone_disp_name[i].ReadExpantion(stream);
		}
	}

	// toon textures
	if (stream->peek() == std::ios::traits_type::eof())
	{
		result->toon_filenames.clear();
	}
	else {
		result->toon_filenames.resize(10);
		for (uint32_t i = 0; i < 10; i++)
		{
			stream->read(buffer, 100);
			result->toon_filenames[i] = std::string(buffer);
		}
	}

	// physics
	if (stream->peek() == std::ios::traits_type::eof())
	{
		result->rigid_bodies.clear();
		result->constraints.clear();
	}
	else {
		uint32_t rigid_body_num;
		stream->read((char*)&rigid_body_num, sizeof(uint32_t));
		result->rigid_bodies.resize(rigid_body_num);
		for (uint32_t i = 0; i < rigid_body_num; i++)
		{
			result->rigid_bodies[i].Read(stream);
		}
		uint32_t constraint_num;
		stream->read((char*)&constraint_num, sizeof(uint32_t));
		result->constraints.resize(constraint_num);
		for (uint32_t i = 0; i < constraint_num; i++)
		{
			result->constraints[i].Read(stream);
		}
	}

	if (stream->peek() != std::ios::traits_type::eof())
	{
		std::cerr << "there is unknown data" << std::endl;
	}

	return result;
}
