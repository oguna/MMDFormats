#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/AnchorRigidBody.h"

namespace pmx
{
	class SoftBody
	{
	public:
		std::string soft_body_name;
		std::string soft_body_english_name;
		uint8_t shape;
		int target_material;
		uint8_t group;
		uint16_t mask;
		SoftBodyFlag flag{ pmx::SoftBodyFlag::BLink };
		int blink_distance;
		int cluster_count;
		float mass;
		float collisioni_margin;
		int aero_model;
		float VCF;
		float DP;
		float DG;
		float LF;
		float PR;
		float VC;
		float DF;
		float MT;
		float CHR;
		float KHR;
		float SHR;
		float AHR;
		float SRHR_CL;
		float SKHR_CL;
		float SSHR_CL;
		float SR_SPLT_CL;
		float SK_SPLT_CL;
		float SS_SPLT_CL;
		int V_IT;
		int P_IT;
		int D_IT;
		int C_IT;
		float LST;
		float AST;
		float VST;
		int anchor_count;
		std::vector<AnchorRigidBody> anchors;
		int pin_vertex_count;
		std::vector<int> pin_vertices;

	public:
		SoftBody() noexcept;
		void parse(std::istream& stream, Setting *setting);
		std::size_t dump(std::ostream& stream, Setting* setting);
	};


}
