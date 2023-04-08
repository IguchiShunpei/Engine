#define DIRECTINPUT_VERSION    0x0800 //DirectInputのバージョン指定
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include"Model.h"

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

	//入力のポインタ
	Input* input = nullptr;
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//カメラ
	ViewProjection* viewProjection = nullptr;
	//カメラ初期化
	viewProjection = new ViewProjection();
	// ビュープロジェクションの初期化
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
	viewProjection->Initialize();
	//スプライトのポインタ
	Sprite* sprite_1 = new Sprite;
	Sprite* sprite_2 = new Sprite;
	//スプライトの初期化
	//1
	sprite_1->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_1->LoadTexture(1, L"Resources/texture.jpg",dxCommon);
	sprite_1->SetPosition({ 0,0,0 });
	//2
	sprite_2->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_2->LoadTexture(1, L"Resources/reimu.png", dxCommon);
	sprite_2->SetPosition({ 100,100,0 });

	//3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//OBJからモデルデータを読み込む
	Model* model_1 = Model:: LoadFromOBJ("triangle_mat");
	Model* model_2 = Model::LoadFromOBJ("player");
	//3Dオブジェクト生成
	Object3d* object3d_1 = Object3d::Create();
	Object3d* object3d_2 = Object3d::Create();
	//オブジェクトにモデルを紐付ける
	object3d_1->SetModel(model_1);
	object3d_2->SetModel(model_2);
	//オブジェクトの位置を指定
	object3d_2->SetPosition({ -5,0,0 });
	//スケールを指定
	object3d_2->SetScale({ 1,1,1 });
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

		// オブジェクト移動
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			// 現在の座標を取得
			Vector3 objPos = object3d_1->GetPosition();

			// 移動後の座標を計算
			if (input->PushKey(DIK_UP)) { objPos.y += 0.1f; }
			else if (input->PushKey(DIK_DOWN)) { objPos.y -= 0.1f; }
			if (input->PushKey(DIK_RIGHT)) { objPos.x += 0.1f; }
			else if (input->PushKey(DIK_LEFT)) { objPos.x -= 0.1f; }

			// 座標の変更を反映
			object3d_1->SetPosition(objPos);
		}

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

		//カメラ
		viewProjection->UpdateMatrix();

		//スプライト更新
		sprite_1->Update();
		sprite_2->Update();

		//3dオブジェクト更新
		object3d_1->Update();
		object3d_2->Update();

#pragma endregion 基盤システムの更新
		
		//描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画
		//3Dオブジェクト描画前処理
		Object3d::PreDraw(dxCommon->GetCommandList());

		object3d_1->Draw(viewProjection);
		object3d_2->Draw(viewProjection);

		//3Dオブジェクト描画前処理
		Object3d::PostDraw();

		//ここにポリゴンなどの描画処理を書く
		sprite_1->Draw(dxCommon);
		sprite_2->Draw(dxCommon);

#pragma endregion 最初のシーンの描画

		// 描画後処理
		dxCommon->PostDraw();
	}

#pragma region 最初のシーンの終了

	//ここにポリゴンの初期化処理を書く

#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了

	//3Dモデル解放
	delete model_1;
	delete model_2;
	//3Dオブジェクト解放
	delete object3d_1;
	delete object3d_2;

	//WindowsAPIの終了処理
	winApp->Finalize();

	//DirectX解放
	delete dxCommon;

	//入力解放
	delete input;

	//スプライトの解放
	delete sprite_1;
	delete sprite_2;

	//WindowsAPI解放
	delete winApp;

#pragma endregion 基盤システムの終了

	return 0;
}
