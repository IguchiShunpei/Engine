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

	//ポストエフェクト用テクスチャ読み込み
	postEffect = new PostEffect();
	postEffect->Initialize(dxCommon);
	postEffect->SetPosition({ 0,0,0 });
}

void SIFrameWork::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();

	// DirectX解放
	dxCommon->fpsFixedFinalize();

	//ポストエフェクト
	delete postEffect;
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

		//レンダーテクスチャへの描画
		//描画前処理
		postEffect->PreDrawScene(dxCommon->GetCommandList());
		// ゲームシーン描画
		Draw();
		//描画後処理
		postEffect->PostDrawScene(dxCommon->GetCommandList());

		//描画前処理
		dxCommon->PreDraw();
		//ポストエフェクト描画
		postEffect->Draw(dxCommon, dxCommon->GetCommandList());
		// 描画後処理
		dxCommon->PostDraw();
	}

	// ゲームの終了
	Finalize();
}
