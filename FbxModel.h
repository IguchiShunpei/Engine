#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

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
public:
	//フレンドクラス
	friend class FbxLoader;
private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
};
