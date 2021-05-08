#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/IkLink.h"

namespace pmx
{
	class Bone
	{
	public:
		/// ボーン名
		std::string bone_name;
		/// ボーン英名
		std::string bone_english_name;
		/// 位置
		float position[3];
		/// 親ボーンインデックス
		int parent_index;
		/// 階層
		int level;
		/// ボーンフラグ
		uint16_t bone_flag;
		/// 座標オフセット(has Target)
		float offset[3];
		/// 接続先ボーンインデックス(not has Target)
		int target_index;
		/// 付与親ボーンインデックス
		int grant_parent_index;
		/// 付与率
		float grant_weight;
		/// 固定軸の方向
		float lock_axis_orientation[3];
		/// ローカル軸のX軸方向
		float local_axis_x_orientation[3];
		/// ローカル軸のY軸方向
		float local_axis_y_orientation[3];
		/// 外部親変形のkey値
		int key;
		/// IKターゲットボーン
		int ik_target_bone_index;
		/// IKループ回数
		int ik_loop;
		/// IKループ計算時の角度制限(ラジアン)
		float ik_loop_angle_limit;
		/// IKリンク数
		int ik_link_count;
		/// IKリンク
		std::vector<IkLink> ik_links;

	public:
		Bone() noexcept;
		void parse(std::istream& stream, pmx::Setting *setting);
		std::size_t dump(std::ostream& stream, pmx::Setting* setting);
	};
}
