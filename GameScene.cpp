#include "GameScene.h"

void GameScene::Initialize()
{
	SIFrameWork::Initialize();
	
	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3Dオブジェクト生成
	fbxObject3d_1 = FbxObject3d::Create();
	//オブジェクトにモデルを紐付ける
	fbxObject3d_1->SetModel(fbxModel_1);
	fbxObject3d_1->SetPosition({ 0,-5,20 });
	fbxObject3d_1->SetRotation({ 0,90,0 });
	fbxObject3d_1->SetScale({ 1,1,1 });
	fbxObject3d_1->PlayAnimation();

	//パーティクル
	particle_1 = Particle::LoadParticleTexture("effect1.png");
	pm_1 = ParticleManager::Create();
	particle_2 = Particle::LoadParticleTexture("effect2.png");
	pm_2 = ParticleManager::Create();
	//オブジェクトにモデルを紐付ける
	pm_1->SetParticleModel(particle_1);
	pm_2->SetParticleModel(particle_2);
	//カメラをセット
	pm_1->SetXMViewProjection(xmViewProjection);
	pm_2->SetXMViewProjection(xmViewProjection);
}

void GameScene::Finalize()
{
	//3Dモデル解放
	delete fbxModel_1;

	//3Dオブジェクト解放
	delete fbxObject3d_1;

	//パーティクル
	delete particle_1;
	delete pm_1;
	delete particle_2;
	delete pm_2;

	//FBX解放
	FbxLoader::GetInstance()->Finalize();

	//カメラ解放
	delete viewProjection;
	delete xmViewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

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

		/*postEffect_1->Update();*/
}

void GameScene::Draw()
{

#pragma region 最初のシーンの描画

	//3Dオブジェクト描画前処理
	FbxObject3d::PreDraw(dxCommon->GetCommandList());

	fbxObject3d_1->Draw(viewProjection);

	//3Dオブジェクト描画前処理
	FbxObject3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	//パーティクル
	pm_1->Draw();
	pm_2->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();

#pragma endregion 最初のシーンの描画

}
