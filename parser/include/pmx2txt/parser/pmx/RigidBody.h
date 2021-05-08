#pragma once

#include <iostream>
#include <string>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class RigidBody
	{
	public:
		/// 剛体名
		std::string rigid_body_name;
		/// 剛体英名
		std::string rigid_body_english_name;
		/// 関連ボーンインデックス
		int target_bone;
		/// グループ
		uint8_t group;
		/// マスク
		uint16_t mask;
		/// 形状
		uint8_t shape;
		float size[3];
		float position[3];
		float orientation[3];
		float mass;
		float move_attenuation;
		float rotation_attenuation;
		float repulsion;
		float friction;
		uint8_t physics_calc_type;

	public:
		RigidBody() noexcept;
		void parse(std::istream& stream, Setting *setting);
		std::size_t dump(std::ostream& stream, Setting* setting);
	};
}
