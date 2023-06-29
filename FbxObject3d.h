#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "FbxModel.h"
#include "FbxLoader.h"

/// 3Dオブジェクト
class FbxObject3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

public:
	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONES];
	};


public: // 静的メンバ関数
	/// 静的初期化
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// 描画後処理
	static void PostDraw();

	/// 3Dオブジェクト生成
	static FbxObject3d* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

private:// 静的メンバ関数

	/// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

public: // メンバ関数

	//初期化
	bool Initialize();

	/// 毎フレーム処理
	void Update();

	/// 描画
	void Draw(ViewProjection* viewProjection);

	//ワールド行列の取得
	const Matrix4& GetMatWorld() { return matWorld; }

	// モデルの設定
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	// オブジェクトの座標
	const Vector3& GetPosition() const { return worldTransform_.position_; }
	const float& GetPositionZ() const { return worldTransform_.position_.z; }
	void SetPosition(const Vector3& position) { this->worldTransform_.position_ = position; }
	// オブジェクトの大きさ
	void SetScale(const Vector3& scale) { this->worldTransform_.scale_ = scale; }
	const Vector3& GetScale() const { return worldTransform_.scale_; }
	// オブジェクトの回転
	void SetRotation(const Vector3& rotation) { this->worldTransform_.rotation_ = rotation; }
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotationX(const float& rotation) { this->worldTransform_.rotation_.x = rotation; }
	void SetRotationY(const float& rotation) { this->worldTransform_.rotation_.y = rotation; }

	//アニメーション
	void PlayAnimation();

public:
	// ワールド変換データ
	WorldTransform worldTransform_;

protected: // メンバ変数
	//クラス名
	const char* name = nullptr;
	// モデル
	FbxModel* fbxModel = nullptr;
	// ローカルワールド変換行列
	Matrix4 matWorld;
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};

