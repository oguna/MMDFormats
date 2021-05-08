#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	/// 枠内要素
	class FrameElement
	{
	public:
		/// 要素対象
		uint8_t element_target;
		/// 要素対象インデックス
		int index;

	public:
		FrameElement() noexcept;
		void parse(std::istream& stream, Setting *setting);
		std::size_t dump(std::ostream& stream, Setting* setting);
	};

	/// 表示枠
	class Frame
	{
	public:
		/// 枠名
		std::string frame_name;
		/// 枠英名
		std::string frame_english_name;
		/// 特殊枠フラグ
		uint8_t frame_flag;
		/// 枠内要素数
		int element_count;
		/// 枠内要素配列
		std::vector<pmx::FrameElement> elements;

	public:
		Frame() noexcept;
		void parse(std::istream& stream, Setting *setting);
		std::size_t dump(std::ostream& stream, Setting* setting);
	};
}
