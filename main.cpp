#define DIRECTINPUT_VERSION    0x0800 //DirectInput�̃o�[�W�����w��
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

	//���͂̃|�C���^
	Input* input = nullptr;
	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);

	//�X�v���C�g�̃|�C���^
	Sprite* sprite_1 = new Sprite;
	Sprite* sprite_2 = new Sprite;
	//�X�v���C�g�̏�����
	//1
	sprite_1->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_1->LoadTexture(1, L"Resources/texture.jpg",dxCommon);
	sprite_1->SetPosition({ 0,0,0 });
	//2
	sprite_2->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	sprite_2->LoadTexture(1, L"Resources/reimu.png", dxCommon);
	sprite_2->SetPosition({ 100,100,0 });

	//3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//OBJ���烂�f���f�[�^��ǂݍ���
	Model* model_1 = Model:: LoadFromOBJ("triangle_mat");
	Model* model_2 = Model::LoadFromOBJ("player");
	//3D�I�u�W�F�N�g����
	Object3d* object3d_1 = Object3d::Create();
	Object3d* object3d_2 = Object3d::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	object3d_1->SetModel(model_1);
	object3d_2->SetModel(model_2);
	//�I�u�W�F�N�g�̈ʒu���w��
	object3d_2->SetPosition({ -20,0,-5 });
	//�X�P�[�����w��
	object3d_2->SetScale({ 10,10,10 });
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
			XMFLOAT3 position = object3d_1->GetPosition();

			// �ړ���̍��W���v�Z
			if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

			// ���W�̕ύX�𔽉f
			object3d_1->SetPosition(position);
		}

		//�X�v���C�g�X�V
		sprite_1->Update();
		sprite_2->Update();

		//3d�I�u�W�F�N�g�X�V
		object3d_1->Update();
		object3d_2->Update();

#pragma endregion ��ՃV�X�e���̍X�V
		
		//�`��O����
		dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
		//3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw(dxCommon->GetCommandList());

		object3d_1->Draw();
		object3d_2->Draw();

		//3D�I�u�W�F�N�g�`��O����
		Object3d::PostDraw();

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
	//3D�I�u�W�F�N�g���
	delete object3d_1;
	delete object3d_2;

	//WindowsAPI�̏I������
	winApp->Finalize();

	//DirectX���
	delete dxCommon;

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
