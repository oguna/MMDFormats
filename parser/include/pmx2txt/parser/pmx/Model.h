#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pmx2txt/parser/pmx/Bone.h"
#include "pmx2txt/parser/pmx/Material.h"
#include "pmx2txt/parser/pmx/Vertex.h"
#include "pmx2txt/parser/pmx/Morph.h"
#include "pmx2txt/parser/pmx/Frame.h"
#include "pmx2txt/parser/pmx/RigidBody.h"
#include "pmx2txt/parser/pmx/SoftBody.h"
#include "pmx2txt/parser/pmx/Joint.h"

namespace pmx
{
	/// PMXモデル
	class Model
	{
	public:
		/// バージョン
		float version;
		/// 設定
		pmx::Setting setting;
		/// モデル名
		std::string model_name;
		/// モデル英名
		std::string model_english_name;
		/// コメント
		std::string model_comment;
		/// 英語コメント
		std::string model_english_comment;
		/// 頂点数
		int vertex_count;
		/// 頂点配列
		std::vector<pmx::Vertex> vertices;
		/// インデックス数
		int index_count;
		/// インデックス配列
		std::vector<int> indices;
		/// テクスチャ数
		int texture_count;
		/// テクスチャ配列
		std::vector<std::string> textures;
		/// マテリアル数
		int material_count;
		/// マテリアル
		std::vector<pmx::Material> materials;
		/// ボーン数
		int bone_count;
		/// ボーン配列
		std::vector<pmx::Bone> bones;
		/// モーフ数
		int morph_count;
		/// モーフ配列
		std::vector<pmx::Morph> morphs;
		/// 表示枠数
		int frame_count;
		/// 表示枠配列
		std::vector<pmx::Frame> frames;
		/// 剛体数
		int rigid_body_count;
		/// 剛体配列
		std::vector<pmx::RigidBody> rigid_bodies;
		/// ジョイント数
		int joint_count;
		/// ジョイント配列
		std::vector<pmx::Joint> joints;
		/// ソフトボディ数
		int soft_body_count;
		/// ソフトボディ配列
		std::vector<pmx::SoftBody> soft_bodies;

	public:
		Model() noexcept;
		/// モデル初期化
		void Init();
		/// モデル読み込み
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}
