#pragma once
#include"Vector3.h"
#include "Vector4.h"
#include"Matrix4.h"
#include "WinApp.h"
#include<d3d12.h>
#include<wrl.h>

class ViewProjection
{
public:// サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataViewProjection {
		Matrix4 view;       // ワールド → ビュー変換行列
		Matrix4 projection; // ビュー → プロジェクション変換行列
		Vector3 cameraPos;  // カメラ座標（ワールド座標）
	};
public:// メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// 初期化
	void Initialize();

	/// 行列を更新する
	void UpdateMatrix();

	// バッファのゲッター
	ID3D12Resource* GetBuff() { return constBuff.Get(); }

	const Vector3& GetEye() { return eye; }

	void SetEye(const Vector3& eye) { this->eye = eye; }

private:// プライベート関数
	// 円周率
	const float PI = 3.141592f;

	/// 定数バッファ生成
	void CreateConstBuffer();

	/// マッピングする
	void Map();

	// 度数からラジアンに変換
	float ToRadian(float angle) { return angle * (PI / 180); }


public:// パブリック変数
#pragma region ビュー行列の設定
	// 視点座標
	Vector3 eye = { 0, 0, -10.0f };
	// 注視点座標
	Vector3 target = { 0, 0, 0 };
	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = ToRadian(45.0f);
	// ビューポートのアスペクト比
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 2000.0f;
#pragma endregion

	// ビュー行列
	Matrix4 matView;
	// 射影行列
	Matrix4 matProjection;

private:// メンバ変数

	// デバイス（借りてくる）
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

};

