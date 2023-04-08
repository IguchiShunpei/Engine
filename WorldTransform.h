#pragma once
#include"Vector3.h"
#include "Vector4.h"
#include"Matrix4.h"
#include<d3d12.h>
#include<wrl.h>

class WorldTransform
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:// サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataB0 {
		Matrix4 matWorld;// 行列
	};
public:// メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// 初期化
	void Initialize();

	/// 行列を更新する
	void UpdateMatrix();

	// 定数バッファのゲッター
	ID3D12Resource* GetBuff() { return constBuffB0.Get(); }

	/// 定数バッファ生成
	void CreateConstBuffer();

	/// マッピングする
	void Map();

	const Vector3& GetPosition()const { return position_; }

	// 度数からラジアンに変換
	float ToRadian(float angle) { return angle * (PI / 180); }

	void SetParent3d(WorldTransform* parent) { this->parent_ = parent; }

public:// パブリック変数
	// ローカルスケール
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };

	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };

	// ローカル座標
	Vector3 position_ = { 0, 0, 0 };

	// 色
	Vector4 color_ = { 1,1,1,1 };

	// ローカル → ワールド変換行列
	Matrix4 matWorld_;

	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	// 円周率
	const float PI = 3.141592f;

private:// メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device_;

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;

	// マッピング済みアドレス
	ConstBufferDataB0* constMap = nullptr;
};

