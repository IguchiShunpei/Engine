#define DIRECTINPUT_VERSION    0x0800 //DirectInputのバージョン指定
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include"Model.h"
#include "ParticleManager.h"
#include "FbxLoader.h"

#include<windows.h>
#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<dinput.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D変換行列
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region 基盤システムの初期化
	//WindowsAPIのポインタ
	WinApp* winApp = nullptr;
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXのポインタ
	DirectXCommon* dxCommon = nullptr;
	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	//入力のポインタ
	Input* input = nullptr;
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//カメラ
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;
	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();
	// ビュープロジェクションの初期化
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
	viewProjection->Initialize();

	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	//FBX
	FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//FBX読み込み
	FbxModel* fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//3Dオブジェクト生成
	FbxObject3d* fbxObject3d_1 = FbxObject3d::Create();
	//オブジェクトにモデルを紐付ける
	fbxObject3d_1->SetModel(fbxModel_1);
	fbxObject3d_1->SetPosition({ 0,-5,20 });
	fbxObject3d_1->SetScale({ 1,1,1 });

	//パーティクル
	Particle* particle_1 = Particle::LoadParticleTexture("effect1.png");
	ParticleManager* pm_1 = ParticleManager::Create();
	Particle* particle_2 = Particle::LoadParticleTexture("effect2.png");
	ParticleManager* pm_2 = ParticleManager::Create();
	//オブジェクトにモデルを紐付ける
	pm_1->SetParticleModel(particle_1);
	pm_2->SetParticleModel(particle_2);
	//カメラをセット
	pm_1->SetXMViewProjection(xmViewProjection);
	pm_2->SetXMViewProjection(xmViewProjection);

#pragma endregion 基盤システムの初期化

	//ゲームループ
	while (true)
	{
#pragma region 基盤システムの更新

		//Windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			//ゲームループを抜ける
			break;
		}

		//入力の更新
		input->Update();

		if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
		{
			// 現在の座標を取得
			Vector3 cameraPos = viewProjection->GetEye();

			// 移動後の座標を計算
			if (input->PushKey(DIK_W)) { cameraPos.y += 0.1f; }
			else if (input->PushKey(DIK_S)) { cameraPos.y -= 0.1f; }
			if (input->PushKey(DIK_A)) { cameraPos.x += 0.1f; }
			else if (input->PushKey(DIK_D)) { cameraPos.x -= 0.1f; }

			// 座標の変更を反映
			viewProjection->SetEye(cameraPos);
		}

		//パーティクル発生
		if (input->PushKey(DIK_SPACE))
		{
			pm_1->Fire(particle_1, 30, 0.2f, 0, 2, { 8.0f, 0.0f });
			pm_2->Fire(particle_2, 70, 0.2f, 0, 5, { 4.0f,0.0f });
		}

		//カメラ
		viewProjection->UpdateMatrix();
		xmViewProjection->Update();

		//fbx更新
		fbxObject3d_1->Update();

		//パーティクル
		pm_1->Update();
		pm_2->Update();

#pragma endregion 基盤システムの更新

		//描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画

		//3Dオブジェクト描画前処理
		FbxObject3d::PreDraw(dxCommon->GetCommandList());

		fbxObject3d_1->Draw(viewProjection);

		//3Dオブジェクト描画前処理
		FbxObject3d::PostDraw();

		//エフェクト描画前処理
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		//パーティクル
		//pm_1->Draw();
		//pm_2->Draw();

		//エフェクト描画後処理
		ParticleManager::PostDraw();

		//ここにポリゴンなどの描画処理を書く
		/*sprite_1->Draw(dxCommon);
		sprite_2->Draw(dxCommon);*/

#pragma endregion 最初のシーンの描画

		// 描画後処理
		dxCommon->PostDraw();
	}

#pragma region 最初のシーンの終了

	//ここにポリゴンの初期化処理を書く

#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了

	//3Dモデル解放
	delete fbxModel_1;

	//3Dオブジェクト解放
	delete fbxObject3d_1;

	//パーティクル
	delete particle_1;
	delete pm_1;
	delete particle_2;
	delete pm_2;

	//WindowsAPIの終了処理
	winApp->Finalize();

	//DirectX解放
	delete dxCommon;

	//FBX解放
	FbxLoader::GetInstance()->Finalize();

	//入力解放
	delete input;

	//WindowsAPI解放
	delete winApp;

#pragma endregion 基盤システムの終了

	return 0;
}
