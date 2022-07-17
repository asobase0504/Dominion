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
std::list<CObject*> CObject::object = {};
int CObject::numAll = 0;
 
//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
CObject::CObject() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0)
{
	object.push_back(this);
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
auto CObject::Release()
{
	for (auto it = object.begin(); it != object.end(); it++)
	{
		if ((*it) != this)
		{
			continue;
		}

		delete this;
		return object.erase(it);
	}
}

//----------------------------------------
// �S�Ă̔j��
//----------------------------------------
void CObject::ReleaseAll()
{
	for (auto it = object.begin(); it != object.end();)
	{
		(*it)->Uninit();

		if (!(*it)->m_enabled)
		{
			it = (*it)->Release();
			continue;
		}

		it++;
	}
}

//----------------------------------------
// �S�Ă̍X�V
//----------------------------------------
void CObject::UpdateAll()
{
	for (auto it = object.begin(); it != object.end();)
	{
		(*it)->Update();

		if (!(*it)->m_enabled)
		{
			it = (*it)->Release();
			continue;
		}

		it++;
	}
}

//----------------------------------------
// �S�Ă̕`��
//----------------------------------------
void CObject::DrawAll()
{
	for (auto it = object.begin(); it != object.end(); it++)
	{
		(*it)->Draw();
	}
}
