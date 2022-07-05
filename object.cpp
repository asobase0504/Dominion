//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#include "object.h"
#include "renderer.h"

//=========================================
// �ÓI�����o�[�ϐ��̐錾
//=========================================
const int CObject::NUM_MAX;
CObject* CObject::object[NUM_MAX] = {};
int CObject::numAll = 0;
 
//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
CObject::CObject() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0)
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] == nullptr)
		{
			numAll++;
			object[i] = this;
			createIdx = i;
			break;
		}
	}
}

//----------------------------------------
// �f�X�g���N�^
//----------------------------------------
CObject::~CObject()
{
}

//----------------------------------------
// �j��
//----------------------------------------
void CObject::Release()
{
	int idx = createIdx;
	if (object[idx] != nullptr)
	{
		object[idx]->Uninit();
		delete object[idx];
		object[idx] = nullptr;
		numAll--;
	}
}

//----------------------------------------
// �S�Ă̔j��
//----------------------------------------
void CObject::ReleaseAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Uninit();
			delete object[i];
			object[i] = nullptr;
		}
	}
}

//----------------------------------------
// �S�Ă̍X�V
//----------------------------------------
void CObject::UpdateAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Update();
		}
	}
}

//----------------------------------------
// �S�Ă̕`��
//----------------------------------------
void CObject::DrawAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Draw();
		}
	}
}