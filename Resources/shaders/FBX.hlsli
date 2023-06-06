cbuffer cbuff0 : register(b0)
{
	matrix matWorld; // ３Ｄ変換行列
};

cbuffer cbuff1 : register(b1)
{
	matrix view;       // ワールド → ビュー変換行列
	matrix projection; // ビュー → プロジェクション変換行列
	vector cameraPos;  // カメラ座標（ワールド座標）
};

//ボーンの最大数
static const int MAX_BONES = 32;

cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL;      // 法線ベクトル
	float2 uv  :TEXCOORD;       // uv値
};

struct VSInput
{
	float4 pos  : POSITION;//位置
	float3 normal : NORMAL;//頂点法線
	float2 uv   : TEXCOORD;//テクスチャ座標
	uint4  boneIndices : BONEINDICES;//ボーンの番号
	float4 boneWeights : BONEWEIGHTS;//ボーンのスキンウェイト
};
