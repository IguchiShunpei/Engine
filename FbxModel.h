#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include<windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>

using namespace DirectX;

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	XMVECTOR scaling = { 1,1,1 };
	//���[�J����]�p
	XMVECTOR rotation = { 0,0,0 };
	//���[�J���ړ�
	XMVECTOR translation = { 0,0,0 };
	//���[�J���ό`�s��
	XMMATRIX transform;
	//�O���[�o���ό`�s��
	XMMATRIX glabalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; //xyz���W
		DirectX::XMFLOAT3 normal; //�@���x�N�g��
		DirectX::XMFLOAT2 uv; //uv���W
	};
public:
	//�t�����h�N���X
	friend class FbxLoader;

public:
	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//���f���̕ό`�s��擾(�萔�o�b�t�@�Ƀ��[���h�s��𑗂邽��)
	const XMMATRIX& getModelTransform() { return meshNode->glabalTransform; }
private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	//���_�o�b�t�@
	ComPtr<ID3D12Resource>vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource>indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};
