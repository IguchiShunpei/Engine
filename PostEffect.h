#pragma once
#include "Sprite.h"

using namespace Microsoft::WRL;

class PostEffect :
    public Sprite
{
public:
    //画面クリアカラー
    static const float clearColor[4];

public:
    //コンストラクタ
    PostEffect();

    //初期化
    void Initialize(DirectXCommon* dxCommon_);

    //描画コマンドの発行
    void Draw(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList);

    //シーン描画前処理
    void PreDrawScene(ID3D12GraphicsCommandList*cmdList);

    //シーン描画後処理
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

