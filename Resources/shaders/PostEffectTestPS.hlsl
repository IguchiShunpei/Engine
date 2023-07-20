#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);     //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float dist = distance(drawUV, pickUV);

	return exp(-(dist * dist) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    //�F���]
   float4 col0 = 1 - tex0.Sample(smp, input.uv);

   //���ςڂ���
   float4 col1 = tex1.Sample(smp, input.uv) * color;
   float U = 5.0f / 1280.0f;
   float V = 5.0f / 720.0f;

   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(-U, -V));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(0.0f, -V));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(U, -V));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(-U, 0.0f));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(0, 0));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(U, 0.0f));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(-U, V));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(0.0f, V));
   // ����s�N�Z���̐F���T���v�����O���A����
   col1 += tex1.Sample(smp, input.uv + float2(U, V));

   // ���v�F��9�Ŋ���
   col1 /= 9.0f;

   //�ȁX�͗l�����
   float4 colortex0 = col0;
   float4 colortex1 = col1;

   float4 color = colortex0;
   if (fmod(input.uv.y, 0.1f) < 0.05f) {
       color = colortex1;
   }

   return float4(color.rgb, 1);
}