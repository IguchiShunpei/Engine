#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    //�R���X�g���N�^
    PostEffect();

    //�`��R�}���h�̔��s
    void Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList);
};

