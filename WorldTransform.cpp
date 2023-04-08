#include "WorldTransform.h"
#include<cassert>
#include <d3dx12.h>

Microsoft::WRL::ComPtr<ID3D12Device> WorldTransform::device_ = nullptr;

void WorldTransform::StaticInitialize(ID3D12Device* device)
{
	assert(device);
	device_ = device;
}

void WorldTransform::Initialize()
{
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	assert(device_);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));

	assert(SUCCEEDED(result));
}

void WorldTransform::Map()
{
	//定数バッファのマッピング
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);//マッピング
	assert(SUCCEEDED(result));
}

void WorldTransform::UpdateMatrix()
{
	HRESULT result;

	Matrix4 matScale, matRot, matTrans;
	Matrix4 matRotX, matRotY, matRotZ;

	//各行列計算
	matScale = Matrix4::identity();
	matScale.scale(scale_);
	matRot = Matrix4::identity();
	matRot *= matRotZ.rotateZ(ToRadian(rotation_.z));
	matRot *= matRotX.rotateX(ToRadian(rotation_.x));
	matRot *= matRotY.rotateY(ToRadian(rotation_.y));
	matTrans = Matrix4::identity();
	matTrans.translate(position_);

	//ワールド行列の合成
	matWorld_ = Matrix4::identity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	//親子構造
	if (parent_ != nullptr) 
	{
		matWorld_ *= parent_->matWorld_;
	}

	//定数バッファに転送
	constMap->matWorld = matWorld_;
}
