#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include<windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include<fbxsdk.h>

using namespace DirectX;

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	Vector4 scaling = { 1,1,1,0 };
	//ローカル回転角
	Vector4 rotation = { 0,0,0,0 };
	//ローカル移動
	Vector4 translation = { 0,0,0,0 };
	//ローカル変形行列
	Matrix4 transform;
	//グローバル変形行列
	Matrix4 glabalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
private://エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public://サブクラス
	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		Vector3 pos; //xyz座標
		Vector3 normal; //法線ベクトル
		Vector2 uv; //uv座標
		UINT boneIndex[MAX_BONE_INDICES];//ボーン番号
		float boneWeight[MAX_BONE_INDICES];//ボーン重量
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		Matrix4 invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
public:
	//フレンドクラス
	friend class FbxLoader;
public:
	//デストラクタ
	~FbxModel();
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列取得(定数バッファにワールド行列を送るため)
	const Matrix4& getModelTransform() { return meshNode->glabalTransform; }
	// setter
	static void SetDevice(ID3D12Device* device) { FbxModel::device = device; }
	//getter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
private:
	// デバイス
	static ID3D12Device* device;
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//ボーン配列
	std::vector<Bone> bones;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;

	//アンビエント係数
	Vector3 ambient = { 1,1,1 };
	Vector3 diffuse = { 1,1,1 };
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
	//FBXシーン
	FbxScene* fbxScene = nullptr;
};
