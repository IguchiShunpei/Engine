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
    //色反転
   float4 col0 = 1 - tex0.Sample(smp, input.uv);

   //平均ぼかし
   float4 col1 = tex1.Sample(smp, input.uv) * color;
   float U = 5.0f / 1280.0f;
   float V = 5.0f / 720.0f;

   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(-U, -V));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(0.0f, -V));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(U, -V));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(-U, 0.0f));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(0, 0));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(U, 0.0f));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(-U, V));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(0.0f, V));
   // 左上ピクセルの色をサンプリングし、足す
   col1 += tex1.Sample(smp, input.uv + float2(U, V));

   // 合計色を9で割る
   col1 /= 9.0f;

   //縞々模様を作る
   float4 colortex0 = col0;
   float4 colortex1 = col1;

   float4 color = colortex0;
   if (fmod(input.uv.y, 0.1f) < 0.05f) {
       color = colortex1;
   }

   return float4(color.rgb, 1);
}