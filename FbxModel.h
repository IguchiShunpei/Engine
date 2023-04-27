#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<DirectXTex.h>

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
};
