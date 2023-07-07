#pragma once
#include "DirectXCommon.h"
#include <wrl.h>
#include<DirectXMath.h>
#include<array>

class Sprite
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス

// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public:
	void Initialize(DirectXCommon*dxCommon_, int window_width, int window_height);

	void Update();

	void Draw(DirectXCommon* dxCommon_);

	void LoadTexture(uint32_t index, const wchar_t* fileName,DirectXCommon* dxCommon_);

	void SetTextureCommands(uint32_t index, DirectXCommon* dxCommon_);

	//座標setter
	void SetPosition(const XMFLOAT3& position) { this->position_ = position; }
	//座標getter
	const XMFLOAT3& GetPosition() const { return position_; }
	//回転setter
	void SetRotation(const XMFLOAT3& rotation) { rotation_ = rotation; }
	//回転getter
	const XMFLOAT3& GetRotation() const { return rotation_; }
	//色setter
	void SetColor(const XMFLOAT4& color) { color_ = color; }
	//色getter
	const XMFLOAT4& GetColor() const { return color_; }
	//表示サイズsetter
	void SetSize(const XMFLOAT2& size) { size_ = size; }
	//表示サイズgetter
	const XMFLOAT2& GetSize() const { return size_; }
	//アンカーポイントsetter
	void SetAnchorPoint(const XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; }
	//アンカーポイントgetter
	const XMFLOAT2& GetAnchorPoint() const { return anchorPoint_; }
	//左右フリップsetter
	bool SetFripX(bool isFlipX) { isFlipX_ = isFlipX; }
	//左右フリップgetter
	bool GetFlipX() const { return isFlipX_; }
	//上下フリップsetter
	bool SetFripY(bool isFlipY) { isFlipY_ = isFlipY; }
	//上下フリップgetter
	bool GetFlipY() const { return isFlipY_; }
	//非表示フラグsetter
	bool SetInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	//非表示フラグgetter
	bool GetInvisible() const { return isInvisible_; }
	//テクスチャ番号setter
	int SetTextureIndex(int textureIndex) { textureIndex_ = textureIndex; }
	//テクスチャ番号getter
	int GetTextureIndex() const { return textureIndex_; }
protected:
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;
	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath;
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;
	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,kMaxSRVCount> textureBuffers_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> srootSignature_;
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> spipelineState_;
	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;

	// 結果確認
	HRESULT result;

	//横方向ピクセル数
	const size_t textureWidth = 256;

	//縦方向ピクセル数
	const size_t textureHeight = 256;

	//座標
	XMFLOAT3 position_ = {0.0f,0.0f,0.0f };
	//スケール
	XMFLOAT3 scale_ = { 1,1,1 };
	//回転
	XMFLOAT3 rotation_ = { 0.0f,0.0f,0.0f };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	//色(RGBA)
	XMFLOAT4 color_ = { 1,1,1,1 };
	//表示サイズ
	XMFLOAT2 size_ = { 100.0f,100.0f };
	//頂点番号
	enum VertexNumber
	{
		LB,  //左下
		LT,  //左上
		RB,  //右下
		RT,  //右上
	};
	//アンカーポイント
	XMFLOAT2 anchorPoint_ = { 0.0f,0.0f };
	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;
	//非表示フラグ
	bool isInvisible_ = false;
	//テクスチャ番号
	uint32_t textureIndex_ = 0;
	//配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;

};

