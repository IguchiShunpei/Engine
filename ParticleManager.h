#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "XMViewProjection.h"
#include "Particle.h"

/// 3Dオブジェクト
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;	//ビルボード行列
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 1024;		// 頂点数
public: // 静的メンバ関数
	/// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// 描画後処理
	static void PostDraw();

	/// 3Dオブジェクト生成
	static ParticleManager* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
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
	bool Initialize();
	/// 毎フレーム処理
	void Update();

	/// 描画
	void Draw();

	/// パーティクル発射
	/// <param name="p">パーティクル</param>
	/// <param name="setpos">位置</param>
	/// <param name="setvel">移動量</param>
	/// <param name="setacc">重力分布</param>
	/// <param name="setnum">一気に何個か</param>
	///  <param name="setscale">x = 開始スケール , y = 終了スケール</param>
	void Active(Particle* p, const float& setpos, const float& setvel, const float& setacc, const int& setnum, const XMFLOAT2& setscale);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// ローカルスケール
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	//パーティクル
	Particle* particle;
	//DirectXMathを使ったViewProjection
	XMViewProjection* xmViewProjection;

public://setter
	//パーティクルモデル
	void SetParticleModel(Particle* particlemodel) { this->particle = particlemodel; }
	//カメラ
	void SetXMViewProjection(XMViewProjection* xmViewProjection) { this->xmViewProjection = xmViewProjection; }
};