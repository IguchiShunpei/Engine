#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

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
public:
	//�t�����h�N���X
	friend class FbxLoader;
private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
};
