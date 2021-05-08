#pragma once

#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class AnchorRigidBody
	{
	public:
		int related_rigid_body;
		int related_vertex;
		bool is_near;

	public:
		AnchorRigidBody() noexcept;
		void parse(std::istream& stream, pmx::Setting *setting);
		std::size_t dump(std::ostream& stream, pmx::Setting* setting);
	};
}
