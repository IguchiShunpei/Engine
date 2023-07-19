#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);     //0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float dist = distance(drawUV, pickUV);

	return exp(-(dist * dist) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 colorTex0 = tex0.Sample(smp, input.uv);
	float4 colorTex1 = tex1.Sample(smp, input.uv);

	float4 color = colorTex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = colorTex1;
	}

	return float4(color.rgb,1);
}