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

	//�p�[�e�B�N���ÓI������
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	//FBX
	FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//FBX�ǂݍ���
	FbxModel* fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//3D�I�u�W�F�N�g����
	FbxObject3d* fbxObject3d_1 = FbxObject3d::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	fbxObject3d_1->SetModel(fbxModel_1);
	fbxObject3d_1->SetPosition({ 0,-5,20 });
	fbxObject3d_1->SetScale({ 1,1,1 });

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
		/*sprite_1->Draw(dxCommon);
		sprite_2->Draw(dxCommon);*/

#pragma endregion �ŏ��̃V�[���̕`��

		// �`��㏈��
		dxCommon->PostDraw();
	}

#pragma region �ŏ��̃V�[���̏I��

	//�����Ƀ|���S���̏���������������

#pragma endregion �ŏ��̃V�[���̏I��

#pragma region ��ՃV�X�e���̏I��

	//3D���f�����
	delete fbxModel_1;

	//3D�I�u�W�F�N�g���
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

	//WindowsAPI���
	delete winApp;

#pragma endregion ��ՃV�X�e���̏I��

	return 0;
}
