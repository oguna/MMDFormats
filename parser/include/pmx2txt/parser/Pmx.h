#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

namespace pmx
{
	/// インデックス設定
	class PmxSetting
	{
	public:
		PmxSetting() noexcept;

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

		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};

	/// 頂点スキニングタイプ
	enum class PmxVertexSkinningType : uint8_t
	{
		BDEF1 = 0,
		BDEF2 = 1,
		BDEF4 = 2,
		SDEF = 3,
		QDEF = 4,
	};

	/// 頂点スキニング
	class PmxVertexSkinning
	{
	public:
		virtual void parse(std::istream& stream, PmxSetting *setting) = 0;
		virtual std::size_t dump(std::ostream& stream, PmxSetting* setting) = 0;
	};

	class PmxVertexSkinningBDEF1 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF1() noexcept;

		int bone_index;
		void parse(std::istream& stresam, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting) override;
	};

	class PmxVertexSkinningBDEF2 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF2() noexcept;

		int bone_index1;
		int bone_index2;
		float bone_weight;
		void parse(std::istream& stresam, PmxSetting *setting)override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting) override;
	};

	class PmxVertexSkinningBDEF4 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF4() noexcept;

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;

		void parse(std::istream& stresam, PmxSetting *setting)override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting) override;
	};

	class PmxVertexSkinningSDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningSDEF() noexcept;

		int bone_index1;
		int bone_index2;
		float bone_weight;
		float sdef_c[3];
		float sdef_r0[3];
		float sdef_r1[3];

		void parse(std::istream& stresam, PmxSetting *setting)override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting) override;
	};

	class PmxVertexSkinningQDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningQDEF() noexcept;

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void parse(std::istream& stresam, PmxSetting *setting)override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting) override;
	};

	/// 頂点
	class PmxVertex
	{
	public:
		PmxVertex() noexcept;

		/// 位置
		float positon[3];
		/// 法線
		float normal[3];
		/// テクスチャ座標
		float uv[2];
		/// 追加テクスチャ座標
		float uva[4][4];
		/// スキニングタイプ
		PmxVertexSkinningType skinning_type{ pmx::PmxVertexSkinningType::BDEF1 };
		/// スキニング
		std::unique_ptr<PmxVertexSkinning> skinning{ nullptr };
		/// エッジ倍率
		float edge;

		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	/// マテリアル
	class PmxMaterial
	{
	public:
		PmxMaterial() noexcept;

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

		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	/// リンク
	class PmxIkLink
	{
	public:
		PmxIkLink() noexcept;

		/// リンクボーンインデックス
		int link_target;
		/// 角度制限
		uint8_t angle_lock;
		/// 最大制限角度
		float max_radian[3];
		/// 最小制限角度
		float min_radian[3];
		void parse(std::istream& stream, PmxSetting *settingn);
		std::size_t dump(std::ostream& stream, PmxSetting* settingn);
	};

	/// ボーン
	class PmxBone
	{
	public:
		PmxBone() noexcept;

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
		std::unique_ptr<PmxIkLink []> ik_links;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	enum class MorphType : uint8_t
	{
		Group = 0,
		Vertex = 1,
		Bone = 2,
		UV = 3,
		AdditionalUV1 = 4,
		AdditionalUV2 = 5,
		AdditionalUV3 = 6,
		AdditionalUV4 = 7,
		Matrial = 8,
		Flip = 9,
		Impulse = 10,
	};

	enum class MorphCategory : uint8_t
	{
		ReservedCategory = 0,
		Eyebrow = 1,
		Eye = 2,
		Mouth = 3,
		Other = 4,
	};

	class PmxMorphOffset
	{
	public:
		void virtual parse(std::istream& stream, PmxSetting *setting) = 0;
		std::size_t virtual dump(std::ostream& stream, PmxSetting* setting)=0;
	};

	class PmxMorphVertexOffset : public PmxMorphOffset
	{
	public:
		PmxMorphVertexOffset() noexcept;
		int vertex_index;
		float position_offset[3];

		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphUVOffset : public PmxMorphOffset
	{
	public:
		PmxMorphUVOffset() noexcept;
		int vertex_index;
		float uv_offset[4];

		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphBoneOffset : public PmxMorphOffset
	{
	public:
		PmxMorphBoneOffset() noexcept;
		int bone_index;
		float translation[3];
		float rotation[4];

		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphMaterialOffset : public PmxMorphOffset
	{
	public:
		PmxMorphMaterialOffset()noexcept;

		int material_index = -1;
		uint8_t offset_operation = -1;
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edge_color[4];
		float edge_size;
		float texture_argb[4];
		float sphere_texture_argb[4];
		float toon_texture_argb[4];
		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphGroupOffset : public PmxMorphOffset
	{
	public:
		PmxMorphGroupOffset() noexcept;
		int morph_index;
		float morph_weight;
		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphFlipOffset : public PmxMorphOffset
	{
	public:
		PmxMorphFlipOffset() noexcept;

		int morph_index;
		float morph_value;
		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	class PmxMorphImpulseOffset : public PmxMorphOffset
	{
	public:
		PmxMorphImpulseOffset() noexcept;

		int rigid_body_index;
		uint8_t is_local;
		float velocity[3];
		float angular_torque[3];
		void parse(std::istream& stream, PmxSetting *setting) override;
		std::size_t dump(std::ostream& stream, PmxSetting* setting)override;
	};

	/// モーフ
	class PmxMorph
	{
	public:
		PmxMorph() noexcept;

		/// モーフ名
		std::string morph_name;
		/// モーフ英名
		std::string morph_english_name;
		/// カテゴリ
		MorphCategory category = static_cast<MorphCategory>(-1);
		/// モーフタイプ
		MorphType morph_type = static_cast<MorphType>(-1);
		/// オフセット数
		int offset_count;
		/// 頂点モーフ配列
		std::unique_ptr<PmxMorphVertexOffset []> vertex_offsets;
		/// UVモーフ配列
		std::unique_ptr<PmxMorphUVOffset []> uv_offsets;
		/// ボーンモーフ配列
		std::unique_ptr<PmxMorphBoneOffset []> bone_offsets;
		/// マテリアルモーフ配列
		std::unique_ptr<PmxMorphMaterialOffset []> material_offsets;
		/// グループモーフ配列
		std::unique_ptr<PmxMorphGroupOffset []> group_offsets;
		/// フリップモーフ配列
		std::unique_ptr<PmxMorphFlipOffset []> flip_offsets;
		/// インパルスモーフ配列
		std::unique_ptr<PmxMorphImpulseOffset []> Impulse_offsets;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	/// 枠内要素
	class PmxFrameElement
	{
	public:
		PmxFrameElement() noexcept;

		/// 要素対象
		uint8_t element_target;
		/// 要素対象インデックス
		int index;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	/// 表示枠
	class PmxFrame
	{
	public:
		PmxFrame() noexcept;

		/// 枠名
		std::string frame_name;
		/// 枠英名
		std::string frame_english_name;
		/// 特殊枠フラグ
		uint8_t frame_flag;
		/// 枠内要素数
		int element_count;
		/// 枠内要素配列
		std::unique_ptr<PmxFrameElement []> elements;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	class PmxRigidBody
	{
	public:
		PmxRigidBody() noexcept;

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
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	enum class PmxJointType : uint8_t
	{
		Generic6DofSpring = 0,
		Generic6Dof = 1,
		Point2Point = 2,
		ConeTwist = 3,
		Slider = 5,
		Hinge = 6
	};

	class PmxJointParam
	{
	public:
		PmxJointParam() noexcept;

		int rigid_body1;
		int rigid_body2;
		float position[3];
		float orientaiton[3];
		float move_limitation_min[3];
		float move_limitation_max[3];
		float rotation_limitation_min[3];
		float rotation_limitation_max[3];
		float spring_move_coefficient[3];
		float spring_rotation_coefficient[3];
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	class PmxJoint
	{
	public:
		std::string joint_name;
		std::string joint_english_name;
		PmxJointType joint_type = static_cast<PmxJointType>(-1);
		PmxJointParam param;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	enum class PmxSoftBodyFlag : uint8_t
	{
		BLink = 0x01,
		Cluster = 0x02,
		Link = 0x04
	};

	class PmxAnchorRigidBody
	{
	public:
		PmxAnchorRigidBody() noexcept;

		int related_rigid_body;
		int related_vertex;
		bool is_near;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	class PmxSoftBody
	{
	public:
		PmxSoftBody() noexcept;

		std::string soft_body_name;
		std::string soft_body_english_name;
		uint8_t shape;
		int target_material;
		uint8_t group;
		uint16_t mask;
		PmxSoftBodyFlag flag{ pmx::PmxSoftBodyFlag::BLink };
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
		std::unique_ptr<PmxAnchorRigidBody []> anchors;
		int pin_vertex_count;
		std::unique_ptr<int []> pin_vertices;
		void parse(std::istream& stream, PmxSetting *setting);
		std::size_t dump(std::ostream& stream, PmxSetting* setting);
	};

	/// PMXモデル
	class PmxModel
	{
	public:
		PmxModel() noexcept;

		/// バージョン
		float version;
		/// 設定
		PmxSetting setting;
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
		std::unique_ptr<PmxVertex []> vertices;
		/// インデックス数
		int index_count;
		/// インデックス配列
		std::unique_ptr<int []> indices;
		/// テクスチャ数
		int texture_count;
		/// テクスチャ配列
		std::unique_ptr< std::string []> textures;
		/// マテリアル数
		int material_count;
		/// マテリアル
		std::unique_ptr<PmxMaterial []> materials;
		/// ボーン数
		int bone_count;
		/// ボーン配列
		std::unique_ptr<PmxBone []> bones;
		/// モーフ数
		int morph_count;
		/// モーフ配列
		std::unique_ptr<PmxMorph []> morphs;
		/// 表示枠数
		int frame_count;
		/// 表示枠配列
		std::unique_ptr<PmxFrame [] > frames;
		/// 剛体数
		int rigid_body_count;
		/// 剛体配列
		std::unique_ptr<PmxRigidBody []> rigid_bodies;
		/// ジョイント数
		int joint_count;
		/// ジョイント配列
		std::unique_ptr<PmxJoint []> joints;
		/// ソフトボディ数
		int soft_body_count;
		/// ソフトボディ配列
		std::unique_ptr<PmxSoftBody []> soft_bodies;
		/// モデル初期化
		void Init();
		/// モデル読み込み
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}
