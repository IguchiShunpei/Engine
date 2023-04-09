#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "XMViewProjection.h"
#include "Particle.h"

/// 3D�I�u�W�F�N�g
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX matBillboard;	//�r���{�[�h�s��
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = 1024;		// ���_��
public: // �ÓI�����o�֐�
	/// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

	/// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// �`��㏈��
	static void PostDraw();

	/// 3D�I�u�W�F�N�g����
	static ParticleManager* Create();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

private:// �ÓI�����o�֐�

	/// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();

public: // �����o�֐�
	bool Initialize();
	/// ���t���[������
	void Update();

	/// �`��
	void Draw();

	/// �p�[�e�B�N������
	/// <param name="p">�p�[�e�B�N��</param>
	/// <param name="setpos">�ʒu</param>
	/// <param name="setvel">�ړ���</param>
	/// <param name="setacc">�d�͕��z</param>
	/// <param name="setnum">��C�ɉ���</param>
	///  <param name="setscale">x = �J�n�X�P�[�� , y = �I���X�P�[��</param>
	void Active(Particle* p, const float& setpos, const float& setvel, const float& setacc, const int& setnum, const XMFLOAT2& setscale);

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	//�p�[�e�B�N��
	Particle* particle;
	//DirectXMath���g����ViewProjection
	XMViewProjection* xmViewProjection;

public://setter
	//�p�[�e�B�N�����f��
	void SetParticleModel(Particle* particlemodel) { this->particle = particlemodel; }
	//�J����
	void SetXMViewProjection(XMViewProjection* xmViewProjection) { this->xmViewProjection = xmViewProjection; }
};