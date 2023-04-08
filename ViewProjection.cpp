#include "ViewProjection.h"
#include <d3dx12.h>
#include <cassert>
#include "WinApp.h"

Microsoft::WRL::ComPtr<ID3D12Device> ViewProjection::device_ = nullptr;

void ViewProjection::StaticInitialize(ID3D12Device* device)
{
	assert(device);
	device_ = device;
}

void ViewProjection::Initialize()
{
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void ViewProjection::CreateConstBuffer()
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

void ViewProjection::Map()
{
	// 定数バッファとのデータリンク
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void ViewProjection::UpdateMatrix()
{
	// ビュー行列の作成
	matView.ViewMat(eye, target, up);
	// 射影行列の作成
	matProjection.ProjectionMat(fovAngleY, aspectRatio, nearZ, farZ);

	// 定数バッファへの書き込み
	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = eye;
}