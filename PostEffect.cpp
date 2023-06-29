#include "PostEffect.h"

#include <d3dx12.h>

PostEffect::PostEffect() : Sprite()
{

}

void PostEffect::Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList)
{
	Update();

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState_.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetTextureCommands(textureIndex_, dxCommon_);

	//頂点バッファビューの設定コマンド
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	//定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
	//描画コマンド
	dxCommon_->GetCommandList()->DrawInstanced(4, 1, 0, 0);  //全ての頂点を使って描画
}
