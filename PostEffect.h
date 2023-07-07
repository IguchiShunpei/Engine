#pragma once
#include "Sprite.h"

using namespace Microsoft::WRL;

class PostEffect :
    public Sprite
{
public:
    //��ʃN���A�J���[
    static const float clearColor[4];

private:
    // DirectX::���ȗ�
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;

public:
    //�R���X�g���N�^
    PostEffect();

    //������
    void Initialize(DirectXCommon* dxCommon_);

    //�p�C�v���C��������
    void CreateGraphicsPipeLineState(DirectXCommon* dxCommon_);

    //�`��R�}���h�̔��s
    void Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList);

    //�V�[���`��O����
    void PreDrawScene(ID3D12GraphicsCommandList*cmdList);

    //�V�[���`��㏈��
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    //���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature_;
    //�p�C�v���C���X�e�[�g
   ComPtr<ID3D12PipelineState> pipelineState_;
};

