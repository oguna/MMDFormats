#pragma once

#include <iostream>
#include <string>

namespace pmx
{
	/// インデックス設定
	class Setting
	{
	public:
		/// エンコード方式
		uint8_t encoding;
		/// 追加UV数
		uint8_t uv;
		/// 頂点インデックスサイズ
		uint8_t vertex_index_size;
		/// テクスチャインデックスサイズ
		uint8_t texture_index_size;
		/// マテリアルインデックスサイズ
		uint8_t material_index_size;
		/// ボーンインデックスサイズ
		uint8_t bone_index_size;
		/// モーフインデックスサイズ
		uint8_t morph_index_size;
		/// 剛体インデックスサイズ
		uint8_t rigidbody_index_size;

	public:
		Setting() noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}
