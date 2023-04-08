#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		Vector3 ambient; // アンビエント係数
		float pad1;       // パディング
		Vector3 diffuse; // ディフューズ係数
		float pad2;       // パディング
		Vector3 specular; // スペキュラー係数
		float alpha;       // アルファ
	};

	// マテリアル
	struct Material
	{
		std::string name; // マテリアル名
		Vector3 ambient; // アンビエント影響度
		Vector3 diffuse; // ディフェーズ影響度
		Vector3 specular;// スペキュラー影響度
		float alpha;      // アルファ
		std::string textureFilename; // テクスチャファイル名
		// コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private: // 定数
	static const int division = 50; // 分割数
	static const float radius; // 底面の半径
	static const float prizmHeight; // 柱の高さ
	static const int planeCount = division * 2 + division * 2; // 面の数
	static const int vertexCount = planeCount * 3; // 頂点数

public: // 静的メンバ関数

	// OBJファイルから3Dモデルを読み込む
	static Model* LoadFromOBJ(const std::string& modelname, const std::string& texname = "Resources");

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	void LoadTexture(const std::string& filename = "Resources");

	// 描画
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, float alpha_ = 1);

	// setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	void SetAlpha(float alpha_);


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// マテリアル
	Material material;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 定数バッファ（マテリアル）
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ

private:// 静的メンバ関数
	// OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal(const std::string& modelname);

	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	// 各種バッファ生成
	void CreateBuffers();

};