//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#include "object.h"
#include "renderer.h"

#include <assert.h>

//=========================================
// �ÓI�����o�[�ϐ��̐錾
//=========================================
const int CObject::NUM_MAX;
std::list<CObject*> CObject::object = {};
int CObject::numAll = 0;
 
//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
CObject::CObject(TYPE type) :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0),
	m_isDeleted(false)
{
	m_type = type;
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

	assert(false);
	return object.begin();
}

//----------------------------------------
// �S�Ă̔j��
//----------------------------------------
void CObject::ReleaseAll()
{
	for (auto it = object.begin(); it != object.end();)
	{
		(*it)->Uninit();

		if (!(*it)->m_isDeleted)
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
	for (auto it = object.begin(); it != object.end(); it++)
	{
		(*it)->Update();
	}

	for (auto it = object.begin(); it != object.end();)
	{
		if ((*it)->m_isDeleted)
		{
			(*it)->Uninit();
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
