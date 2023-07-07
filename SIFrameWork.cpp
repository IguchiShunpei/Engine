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
	postEffect = new PostEffect();
	postEffect->Initialize(dxCommon);
	postEffect->SetPosition({ 0,0,0 });
}

void SIFrameWork::Finalize()
{
	// WindowsAPI�̏I������
	winApp->Finalize();

	// DirectX���
	dxCommon->fpsFixedFinalize();

	//�|�X�g�G�t�F�N�g
	delete postEffect;
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

		//�����_�[�e�N�X�`���ւ̕`��
		//�`��O����
		postEffect->PreDrawScene(dxCommon->GetCommandList());
		// �Q�[���V�[���`��
		Draw();
		//�`��㏈��
		postEffect->PostDrawScene(dxCommon->GetCommandList());

		//�`��O����
		dxCommon->PreDraw();
		//�|�X�g�G�t�F�N�g�`��
		postEffect->Draw(dxCommon, dxCommon->GetCommandList());
		// �`��㏈��
		dxCommon->PostDraw();
	}

	// �Q�[���̏I��
	Finalize();
}
