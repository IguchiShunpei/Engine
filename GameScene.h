#pragma once
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class GameScene : public SIFrameWork
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;

private:
	//入力
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//カメラ
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;
	//FBX読み込み
	FbxModel* fbxModel_1 = nullptr;
	//FBXモデル
	FbxObject3d* fbxObject3d_1 = nullptr;
	//パーティクル
	Particle* particle_1 = nullptr;
	ParticleManager* pm_1 = nullptr;
	Particle* particle_2 = nullptr;
	ParticleManager* pm_2 = nullptr;
};
