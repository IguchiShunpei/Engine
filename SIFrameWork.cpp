#include "SIFrameWork.h"

void SIFrameWork::Initialize()
{
	//WindowsAPI�̏�����
	winApp = WinApp::GetInstance();;
	winApp->Initialize();
	//DirectX�̏�����
	dxCommon = DirectXCommon::GetInstance();;
	dxCommon->Initialize(winApp);
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);
	//FBX
	FbxObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	// �p�[�e�B�N���ÓI������
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	// �r���[�v���W�F�N�V�����̏�����
	ViewProjection::StaticInitialize(dxCommon->GetDevice());

	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂݍ���
	postEffect_1 = new PostEffect();
	postEffect_1->Initialize(dxCommon, WinApp::window_width, WinApp::window_height);
	postEffect_1->LoadTexture(1, L"Resources/texture.jpg", dxCommon);
	postEffect_1->SetPosition({ 0,0,0 });
}

void SIFrameWork::Finalize()
{
	// WindowsAPI�̏I������
	winApp->Finalize();

	// DirectX���
	dxCommon->fpsFixedFinalize();

	//�|�X�g�G�t�F�N�g
	delete postEffect_1;
}

void SIFrameWork::Update()
{
	// Windows�̃��b�Z�[�W����
	if (winApp->ProcessMessage()) {
		// �Q�[�����[�v�𔲂���
		endRequest = true;
	}

	// ���͂̍X�V
	input->Update();
}

void SIFrameWork::Run()
{
	// �Q�[���̏�����
	Initialize();

	while (true) // �Q�[�����[�v 
	{
		// ���t���[���X�V
		Update();

		// �I�����N�G�X�g�������烋�[�v�𔲂���
		if (IsEndRequest()) {
			// �Q�[�����[�v�𔲂���
			break;
		}

		//�`��O����
		dxCommon->PreDraw();

		//�|�X�g�G�t�F�N�g�`��
		postEffect_1->Draw(dxCommon, dxCommon->GetCommandList());

		//// �Q�[���V�[���`��
		//Draw();

		// �`��㏈��
		dxCommon->PostDraw();
	}

	// �Q�[���̏I��
	Finalize();
}
