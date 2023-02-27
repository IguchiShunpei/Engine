//マテリアル
//ピクセルシェーダ
cbuffer ConstBufferDateMaterial : register(b0)
{
	float4 color;
}
cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;
}
//頂点シェーダーの出力構造体
//(頂点シェーダからピクセルシェーダへのやり取りに使用する)
struct VSOutput
{
	float4 svpos : SV_POSITION;//システム用頂点座標
	float2 uv : TEXCOORD;//uv値
};