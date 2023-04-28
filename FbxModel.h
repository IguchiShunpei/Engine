#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include<windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>

using namespace DirectX;

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	XMVECTOR scaling = { 1,1,1 };
	//ローカル回転角
	XMVECTOR rotation = { 0,0,0 };
	//ローカル移動
	XMVECTOR translation = { 0,0,0 };
	//ローカル変形行列
	XMMATRIX transform;
	//グローバル変形行列
	XMMATRIX glabalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
private://エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; //xyz座標
		DirectX::XMFLOAT3 normal; //法線ベクトル
		DirectX::XMFLOAT2 uv; //uv座標
	};
public:
	//フレンドクラス
	friend class FbxLoader;

public:
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列取得(定数バッファにワールド行列を送るため)
	const XMMATRIX& getModelTransform() { return meshNode->glabalTransform; }
private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;

	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	//頂点バッファ
	ComPtr<ID3D12Resource>vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource>indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource>texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};
