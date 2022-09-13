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
std::vector<std::list<CObject*>> CObject::object = {};
int CObject::numAll = 0;
bool CObject::isStopUpdate = false;

//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
CObject::CObject(TYPE type, int priority) :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0),
	m_isDeleted(false),
	shouldStopAlsoUpdate(false)
{
	m_type = type;

	if ((int)object.size() < priority + 1)
	{
		object.resize(priority + 1);
	}
	object.at(priority).push_back(this);
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
	for (int i = 0;i < (int)object.size();i++)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			if ((*it) != this)
			{
				continue;
			}

			delete this;
			return object.at(i).erase(it);
		}
	}

	assert(false);
	return object.at(0).begin();
}

//----------------------------------------
// �S�Ă̔j��
//----------------------------------------
void CObject::ReleaseAll()
{
	for (int i = (int)object.size() - 1; i >= 0; i--)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end();)
		{
			(*it)->Uninit();

			if ((*it)->m_isDeleted)
			{
				delete (*it);
				(*it) = nullptr;
				it = object.at(i).erase(it);
				continue;
			}

			it++;
		}
	}
}

//----------------------------------------
// �S�Ă̍X�V
//----------------------------------------
void CObject::UpdateAll()
{
	for (int i = 0; i < (int)object.size(); i++)
	{
		// �X�V����
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			if ((((*it)->shouldStopAlsoUpdate && isStopUpdate) || !isStopUpdate) && !(*it)->m_isDeleted)
			{
				(*it)->Update();
			}
		}

		// ���S����
		for (auto it = object.at(i).begin(); it != object.at(i).end();)
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
}

//----------------------------------------
// �S�Ă̕`��
//----------------------------------------
void CObject::DrawAll()
{
	for (int i = 0; i < (int)object.size(); i++)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			(*it)->Draw();
		}
	}
}
