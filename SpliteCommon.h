#include "DirectXCommon.h"
#include "wrl.h"
#include <DirectXMath.h>

using namespace DirectX;

#pragma once
class SpliteCommon
{

public:
	//namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//初期化
	void Initialize(DirectXCommon*dxCommon);

	//getter
	DirectXCommon* GetDxCommon() const { return dxCommon; }

private://静的メンバ変数
	//デバイス
	static ID3D12Device*device;

private://メンバ変数
	// DirectXCommonのインスタンス
	DirectXCommon* dxCommon = nullptr;
};

