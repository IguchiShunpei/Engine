#include "SIFrameWork.h"

void SIFrameWork::Initialize()
{
	//WindowsAPIの初期化
	winApp = WinApp::GetInstance();;
	winApp->Initialize();
	//DirectXの初期化
	dxCommon = DirectXCommon::GetInstance();;
	dxCommon->Initialize(winApp);
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);
	//FBX
	FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	// ビュープロジェクションの初期化
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
}

void SIFrameWork::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();

	// DirectX解放
	dxCommon->fpsFixedFinalize();
}

void SIFrameWork::Update()
{
	// Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		// ゲームループを抜ける
		endRequest = true;
	}

	// 入力の更新
	input->Update();
}

void SIFrameWork::Run()
{
	// ゲームの初期化
	Initialize();

	while (true) // ゲームループ 
	{
		// 毎フレーム更新
		Update();

		// 終了リクエストが来たらループを抜ける
		if (IsEndRequest()) {
			// ゲームループを抜ける
			break;
		}

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();
}
