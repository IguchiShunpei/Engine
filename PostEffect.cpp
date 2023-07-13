#include "PostEffect.h"
#include "WinApp.h"

#include<d3dcompiler.h>
#include <d3dx12.h>

#pragma comment(lib,"d3dcompiler.lib")

//静的メンバ変数の実体
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };//緑っぽい色

using namespace DirectX;
using namespace Microsoft::WRL;

struct Vertex
{
	XMFLOAT3 pos; //xyz座標

	XMFLOAT2 uv;  //uv座標
};

PostEffect::PostEffect() : Sprite()
{

}

void PostEffect::Initialize(DirectXCommon* dxCommon_)
{
	input = Input::GetInstance();

	HRESULT result;

	//頂点データ
	Vertex vertices[] = {
		{{-0.5f,-0.5f,0.0f},{0.0f,1.0f,}},//左下  インデックス0
		{{-0.5f,+0.5f,0.0f},{0.0f,0.0f,}},//左上  インデックス1
		{{+0.5f,-0.5f,0.0f},{1.0f,1.0f,}},//右下  インデックス2
		{{+0.5f,+0.5f,0.0f},{1.0f,0.0f,}},//右上  インデックス3
	};

	//頂点データの全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};        //ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;  //GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの作成
	result = dxCommon_->GetDevice()->CreateCommittedResource
	(
		&heapProp,  //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,   //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//前頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   //座標をコピー
	}

	//繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//CBV,SRV,UAVの1個分のサイズを取得
	UINT descriptorSize = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;//色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		XMMATRIX mat; //3D変換行列
	};

	//マテリアル//

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;  //GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;  //256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	//result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される
	//constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);  //RGBAで半透明の赤

	////3D変換行列////
	ConstBufferDataTransform* constMapTransform = nullptr; //定数バッファのマッピング用ポインタ

	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff; //256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//定数バッファの生成
		result = dxCommon_->GetDevice()->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform);  //マッピング
		assert(SUCCEEDED(result));

		//単位行列を代入
		matProjection = XMMatrixOrthographicOffCenterLH(
			0.0f, (float)WinApp::window_width,
			(float)WinApp::window_height, 0.0f
			, 0.0f, 1.0f);
	}

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES texHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE texClearVal = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	for (int i = 0; i < 2; i++)
	{
		//テクスチャバッファの生成
		result = dxCommon_->GetDevice()->CreateCommittedResource(
			&texHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&texClearVal,
			IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result));

		{//テクスチャを赤クリア
			//画素数
			const UINT pixelCount = WinApp::window_width * WinApp::window_height;
			//画像1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * WinApp::window_height;
			//画像イメージ
			UINT* img = new UINT[pixelCount];
			for (int j = 0; j < pixelCount; j++)
			{
				img[j] = 0xff0000ff;
			}
			//テクスチャバッファにデータ転送
			result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}
	}
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	dxCommon_->GetDevice()->CreateShaderResourceView(texBuff[0].Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV用デスクリプタヒープを生成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < 2; i++)
	{
		//デスクリプタヒープにRTV作成
		dxCommon_->GetDevice()->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			)
		);
	}

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//深度バッファの生成
	CD3DX12_HEAP_PROPERTIES depthHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthVal = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&depthHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthVal,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCommon_->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	CreateGraphicsPipeLineState(dxCommon_);
}

void PostEffect::CreateGraphicsPipeLineState(DirectXCommon* dxCommon_)
{
	HRESULT result = S_FALSE;

	ComPtr < ID3DBlob> vsBlob;    //頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob;    //ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob; //エラーオブジェクト

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile
	(
		L"Resources/shaders/PostEffectTestVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  //インクルード可能にする
		"main", "vs_5_0",  //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,  //デバッグ用指定
		0,
		&vsBlob, &errorBlob
	);

	//エラーなら
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile
	(
		L"Resources/shaders/PostEffectTestPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  //インクルード可能にする
		"main", "ps_5_0",  //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,  //デバッグ用指定
		0,
		&psBlob, &errorBlob
	);

	//エラーなら
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			//xyz座標(1行で書いたほうが見やすい)
		   "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			//uv座標(1行で書いたほうが見やすい)
		   "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//シェーダーの設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライザの設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;   //カリングしない

	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blenddesc.BlendEnable = true; //ブレンドを有効にする	

	//半透明合成	
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //加算	
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //ソースのアルファ値	
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f-ソースのアルファ値

	//共通設定(アルファ値)	
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算	
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; //ソースの値を100% 使う	
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デストの値を 0% 使う	

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;  //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //0~255指定のRGBA
	gpipeline.SampleDesc.Count = 1;  //1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0レジスタ

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob_;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob_, &errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャの生成
	result = dxCommon_->GetDevice()->CreateRootSignature(0, rootSigBlob_->GetBufferPointer(), rootSigBlob_->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootSignature_.Get();

	//パイプラインステートを生成
	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));

}

void PostEffect::Draw(DirectXCommon* dxCommon_, ID3D12GraphicsCommandList* cmdList)
{
	//入力の更新
	input->Update();

	if (input->TriggerKey(DIK_0))
	{
		//デスクリプタヒープにSRV作成
		static int tex = 0;
		//テクスチャ番号を0と1で切り替え
		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;
		dxCommon_->GetDevice()->CreateShaderResourceView(texBuff[tex].Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()
			);
	}

	//転送
	ConstBufferDataB0* constMap0 = nullptr;
	result = this->constBuffTransform_->Map(0, nullptr, (void**)&constMap0);
	constMap0->mat = XMMatrixIdentity();
	constBuffTransform_->Unmap(0, nullptr);

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState_.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//頂点バッファビューの設定コマンド
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	//定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
	//描画コマンド
	dxCommon_->GetCommandList()->DrawInstanced(4, 1, 0, 0);  //全ての頂点を使って描画
}

void PostEffect::PreDrawScene(DirectXCommon* dxCommon_,ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < 2; i++)
	{
		CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

		//リソースバリアを変更
		cmdList->ResourceBarrier(1, &resBarrier);
	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
	for (int i = 0; i < 2; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	CD3DX12_VIEWPORT viewPorts[2];
	CD3DX12_RECT rects[2];

	for (int i = 0; i < 2; i++)
	{
	viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	rects[i] = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
	}

	//ビューポートの設定
	cmdList->RSSetViewports(2, viewPorts);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(2, rects);

	for (int i = 0; i < 2; i++)
	{
	//全画面クリア
	cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < 2; i++)
	{
	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//リソースバリアを変更(描画可能→シェーダーリソース)
	cmdList->ResourceBarrier(1, &resBarrier);
	}
}
