#pragma once

#include <iostream>
#include <string>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	/// マテリアル
	class Material
	{
	public:
		/// モデル名
		std::string material_name;
		/// モデル英名
		std::string material_english_name;
		/// 減衰色
		float diffuse[4];
		/// 光沢色
		float specular[3];
		/// 光沢度
		float specularlity;
		/// 環境色
		float ambient[3];
		/// 描画フラグ
		uint8_t flag;
		/// エッジ色
		float edge_color[4];
		/// エッジサイズ
		float edge_size;
		/// アルベドテクスチャインデックス
		int diffuse_texture_index;
		/// スフィアテクスチャインデックス
		int sphere_texture_index;
		/// スフィアテクスチャ演算モード
		uint8_t sphere_op_mode;
		/// 共有トゥーンフラグ
		uint8_t common_toon_flag;
		/// トゥーンテクスチャインデックス
		int toon_texture_index;
		/// メモ
		std::string memo;
		/// 頂点インデックス数
		int index_count;

	public:
		Material() noexcept;
		void parse(std::istream& stream, Setting *setting);
		std::size_t dump(std::ostream& stream, Setting* setting);
	};
}
