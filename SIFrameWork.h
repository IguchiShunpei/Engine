#pragma once
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include"Model.h"
#include "ParticleManager.h"
#include "FbxLoader.h"

class SIFrameWork
{
public://メンバ関数
	//初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw() = 0;
	//終了チェック
	virtual bool IsEndRequest() { return endRequest; }

public:
	virtual ~SIFrameWork() = default;

public:
	//実行
	void Run();

private:
	//WindowsAPIのポインタ
	WinApp* winApp = nullptr;
	//DirectXのポインタ
	DirectXCommon* dxCommon = nullptr;
	//入力のポインタ
	Input* input = nullptr;
private:
	bool endRequest = false;
};

