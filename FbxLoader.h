#pragma once
#include "fbxsdk.h"
#include "FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{
private://エイリアス
	//stdを省略
	using string = std::string;
public:
	/// シングルトンインスタンスの取得
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	//初期化
	void Initialize(ID3D12Device* device);

	//後始末
	void Finalize();

	//ファイルからFBXモデル読み込み
	void LoadModelFromFile(const string& modelName);

	//再帰的にノード構成を解析
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode,Node* parent = nullptr);
	
	//メッシュ読み取り
	//"fbxModel">読み込み先オブジェクト
	//"fbxNode">解析対象のノード
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);

	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//頂点座標読み取り
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//頂点座標読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//頂点座標読み取り
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

public://定数
	//モデル格納ルートパス
	static const string baseDirectory;

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

};