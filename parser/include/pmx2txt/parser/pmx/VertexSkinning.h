#pragma once

#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class VertexSkinning
	{
	public:
		virtual void parse(std::istream& stream, Setting *setting) = 0;
		virtual std::size_t dump(std::ostream& stream, Setting* setting) = 0;
	};

	class VertexSkinningBDEF1 : public VertexSkinning
	{
	public:
		int bone_index;

	public:
		VertexSkinningBDEF1() noexcept;
		void parse(std::istream& stresam, Setting *setting) override;
		std::size_t dump(std::ostream& stream, Setting* setting) override;
	};

	class VertexSkinningBDEF2 : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;

	public:
		VertexSkinningBDEF2() noexcept;
		void parse(std::istream& stresam, Setting *setting)override;
		std::size_t dump(std::ostream& stream, Setting* setting) override;
	};

	class VertexSkinningBDEF4 : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;

	public:
		VertexSkinningBDEF4() noexcept;
		void parse(std::istream& stresam, Setting *setting)override;
		std::size_t dump(std::ostream& stream, Setting* setting) override;
	};

	class VertexSkinningSDEF : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;
		float sdef_c[3];
		float sdef_r0[3];
		float sdef_r1[3];

	public:
		VertexSkinningSDEF() noexcept;
		void parse(std::istream& stresam, Setting *setting)override;
		std::size_t dump(std::ostream& stream, Setting* setting) override;
	};

	class VertexSkinningQDEF : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;

	public:
		VertexSkinningQDEF() noexcept;
		void parse(std::istream& stresam, Setting *setting)override;
		std::size_t dump(std::ostream& stream, Setting* setting) override;
	};
}
