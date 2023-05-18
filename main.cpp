#define DIRECTINPUT_VERSION    0x0800 //DirectInput�̃o�[�W�����w��
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

// �E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;//�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D�ϊ��s��
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region ��ՃV�X�e���̏�����
	//WindowsAPI�̃|�C���^
	WinApp* winApp = nullptr;
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX�̃|�C���^
	DirectXCommon* dxCommon = nullptr;
	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	//���͂̃|�C���^
	Input* input = nullptr;
	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);

	//�J����
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;
	//�J����������
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();
	// �r���[�v���W�F�N�V�����̏�����
	ViewProjection::StaticInitialize(dxCommon->GetDevice());
	viewProjection->Initialize();

	//�X�v���C�g�̃|�C���^
	Sprite* sprite_1 = new Sprite;
	Sprite* sprite_2 = new Sprite;
	//�X�v���C�g�̏�����
	//1
	sprite_1->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_1->LoadTexture(1, L"Resources/texture.jpg", dxCommon);
	sprite_1->SetPosition({ 0,0,0 });
	//2
	sprite_2->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_2->LoadTexture(1, L"Resources/reimu.png", dxCommon);
	sprite_2->SetPosition({ 100,100,0 });

	//3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model_1 = Model::LoadFromOBJ("triangle_mat");
	Model* model_2 = Model::LoadFromOBJ("player");
	//3D�I�u�W�F�N�g����
	Object3d* object3d_1 = Object3d::Create();
	Object3d* object3d_2 = Object3d::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	object3d_1->SetModel(model_1);
	object3d_2->SetModel(model_2);
	//�I�u�W�F�N�g�̈ʒu���w��
	object3d_2->SetPosition({ -5,0,0 });
	//�X�P�[�����w��
	object3d_2->SetScale({ 1,1,1 });

	//FBX
	FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//FBX�ǂݍ���
	FbxModel* fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	//3D�I�u�W�F�N�g����
	FbxObject3d* fbxObject3d_1 = FbxObject3d::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	fbxObject3d_1->SetModel(fbxModel_1);
	fbxObject3d_1->SetPosition({ 0,-5,20 });
	fbxObject3d_1->SetScale({ 0.1,0.1,0.1 });

	//�p�[�e�B�N��
	Particle* particle_1 = Particle::LoadParticleTexture("effect1.png");
	ParticleManager* pm_1 = ParticleManager::Create();
	Particle* particle_2 = Particle::LoadParticleTexture("effect2.png");
	ParticleManager* pm_2 = ParticleManager::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	pm_1->SetParticleModel(particle_1);
	pm_2->SetParticleModel(particle_2);
	//�J�������Z�b�g
	pm_1->SetXMViewProjection(xmViewProjection);
	pm_2->SetXMViewProjection(xmViewProjection);

#pragma endregion ��ՃV�X�e���̏�����

	//�Q�[�����[�v
	while (true)
	{
#pragma region ��ՃV�X�e���̍X�V

		//Windows�̃��b�Z�[�W����
		if (winApp->ProcessMessage())
		{
			//�Q�[�����[�v�𔲂���
			break;
		}

		//���͂̍X�V
		input->Update();

		// �I�u�W�F�N�g�ړ�
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			// ���݂̍��W���擾
			Vector3 objPos = object3d_1->GetPosition();

			// �ړ���̍��W���v�Z
			if (input->PushKey(DIK_UP)) { objPos.y += 0.1f; }
			else if (input->PushKey(DIK_DOWN)) { objPos.y -= 0.1f; }
			if (input->PushKey(DIK_RIGHT)) { objPos.x += 0.1f; }
			else if (input->PushKey(DIK_LEFT)) { objPos.x -= 0.1f; }

			// ���W�̕ύX�𔽉f
			object3d_1->SetPosition(objPos);
		}

		if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
		{
			// ���݂̍��W���擾
			Vector3 cameraPos = viewProjection->GetEye();

			// �ړ���̍��W���v�Z
			if (input->PushKey(DIK_W)) { cameraPos.y += 0.1f; }
			else if (input->PushKey(DIK_S)) { cameraPos.y -= 0.1f; }
			if (input->PushKey(DIK_A)) { cameraPos.x += 0.1f; }
			else if (input->PushKey(DIK_D)) { cameraPos.x -= 0.1f; }

			// ���W�̕ύX�𔽉f
			viewProjection->SetEye(cameraPos);
		}

		//�p�[�e�B�N������
		if (input->PushKey(DIK_SPACE))
		{
			pm_1->Fire(particle_1, 30, 0.2f, 0, 2, { 8.0f, 0.0f });
			pm_2->Fire(particle_2, 70, 0.2f, 0, 5, { 4.0f,0.0f });
		}

		//�J����
		viewProjection->UpdateMatrix();
		xmViewProjection->Update();

		//�X�v���C�g�X�V
		sprite_1->Update();
		sprite_2->Update();

		//3d�I�u�W�F�N�g�X�V
		object3d_1->Update();
		object3d_2->Update();

		//fbx�X�V
		fbxObject3d_1->Update();

		//�p�[�e�B�N��
		pm_1->Update();
		pm_2->Update();

#pragma endregion ��ՃV�X�e���̍X�V

		//�`��O����
		dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		//3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw(dxCommon->GetCommandList());

		//�I�u�W�F�N�g
		object3d_1->Draw(viewProjection);
		object3d_2->Draw(viewProjection);

		//3D�I�u�W�F�N�g�`��O����
		Object3d::PostDraw();

		//3D�I�u�W�F�N�g�`��O����
		FbxObject3d::PreDraw(dxCommon->GetCommandList());

		fbxObject3d_1->Draw(viewProjection);

		//3D�I�u�W�F�N�g�`��O����
		FbxObject3d::PostDraw();

		//�G�t�F�N�g�`��O����
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		//�p�[�e�B�N��
		//pm_1->Draw();
		//pm_2->Draw();

		//�G�t�F�N�g�`��㏈��
		ParticleManager::PostDraw();

		//�����Ƀ|���S���Ȃǂ̕`�揈��������
		sprite_1->Draw(dxCommon);
		sprite_2->Draw(dxCommon);

#pragma endregion �ŏ��̃V�[���̕`��

		// �`��㏈��
		dxCommon->PostDraw();
	}

#pragma region �ŏ��̃V�[���̏I��

	//�����Ƀ|���S���̏���������������

#pragma endregion �ŏ��̃V�[���̏I��

#pragma region ��ՃV�X�e���̏I��

	//3D���f�����
	delete model_1;
	delete model_2;
	delete fbxModel_1;

	//3D�I�u�W�F�N�g���
	delete object3d_1;
	delete object3d_2;
	delete fbxObject3d_1;

	//�p�[�e�B�N��
	delete particle_1;
	delete pm_1;
	delete particle_2;
	delete pm_2;

	//WindowsAPI�̏I������
	winApp->Finalize();

	//DirectX���
	delete dxCommon;

	//FBX���
	FbxLoader::GetInstance()->Finalize();

	//���͉��
	delete input;

	//�X�v���C�g�̉��
	delete sprite_1;
	delete sprite_2;

	//WindowsAPI���
	delete winApp;

#pragma endregion ��ՃV�X�e���̏I��

	return 0;
}
