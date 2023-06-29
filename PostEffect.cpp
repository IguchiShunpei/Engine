#include "PostEffect.h"

#include <d3dx12.h>

PostEffect::PostEffect() : Sprite()
{

}

void PostEffect::Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList)
{
	Update();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState_.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetTextureCommands(textureIndex_, dxCommon_);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
	//�`��R�}���h
	dxCommon_->GetCommandList()->DrawInstanced(4, 1, 0, 0);  //�S�Ă̒��_���g���ĕ`��
}
