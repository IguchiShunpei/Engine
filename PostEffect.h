#pragma once
#include "Sprite.h"

using namespace Microsoft::WRL;

class PostEffect :
    public Sprite
{
public:
    //�R���X�g���N�^
    PostEffect();

    //������
    void Initialize(DirectXCommon* dxCommon_);

    //�`��R�}���h�̔��s
    void Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList);

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

