#pragma once

#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	/// リンク
	class IkLink
	{
	public:
		IkLink() noexcept;

		/// リンクボーンインデックス
		int link_target;
		/// 角度制限
		uint8_t angle_lock;
		/// 最大制限角度
		float max_radian[3];
		/// 最小制限角度
		float min_radian[3];
		void parse(std::istream& stream, Setting *settingn);
		std::size_t dump(std::ostream& stream, Setting* settingn);
	};
}
