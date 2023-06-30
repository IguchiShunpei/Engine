#pragma once
#include "Sprite.h"

using namespace Microsoft::WRL;

class PostEffect :
    public Sprite
{
public:
    //コンストラクタ
    PostEffect();

    //初期化
    void Initialize(DirectXCommon* dxCommon_);

    //描画コマンドの発行
    void Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList);

private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

