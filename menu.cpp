//=========================================
// 
// ���j���[�N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "menu.h"
#include "application.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CMenu::CMenu() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Area(D3DXVECTOR2(0.0f, 0.0f)),
	m_fream(nullptr),
	m_item({}),
	m_selectIdx({0,0}),
	m_fInterval(D3DXVECTOR2(0.0f, 0.0f)),
	m_AroundWhitespace(D3DXVECTOR2(0.0f, 0.0f))
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CMenu::~CMenu()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CMenu::Init()
{
	m_fInterval = { 5.0f,25.0f };
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CMenu::Uninit()
{
	m_item.clear();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CMenu::Update()
{
	for (int i = 0; i < m_item.size(); i++)
	{
		for (int j = 0; j < m_item[i].size(); j++)
		{
			if (m_selectIdx[0] == i && m_selectIdx[1] == j)
			{
				m_item[i][j]->SelectUpdate();
			}
			else
			{
				m_item[i][j]->NomalUpdate();
			}
		}
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CMenu::Draw()
{
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CMenu::Select(SELECT_DIRECTION inDirection)
{
	switch (inDirection)
	{
	case CMenu::TOP:
		m_selectIdx[0]--;
		if (m_selectIdx[0] < 0)
		{
			m_selectIdx[0] = (int)m_item.size() - 1;
		}
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::DOWN:
		m_selectIdx[0]++;
		if (m_selectIdx[0] >= (int)m_item.size())
		{
			m_selectIdx[0] = 0;
		}
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::LEFT:
		m_selectIdx[1]--;
		if (m_selectIdx[1] < 0)
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::RIGHT:
		m_selectIdx[1]++;
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = 0;
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------
// �I�����ꂽ�ԍ��̐ݒ�
//-----------------------------------------
void CMenu::SetSelectIdx(int Y, int X)
{
	m_selectIdx[0] = Y;
	m_selectIdx[1] = X;
}

//-----------------------------------------
// �I�����ꂽ�ԍ��̐ݒ�
//-----------------------------------------
void CMenu::SetSelectIdx(std::vector<int> inIdx)
{
	m_selectIdx = inIdx;
}

//-----------------------------------------
// �쐬
//-----------------------------------------
CMenu * CMenu::Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea, CMenuFream * inFream, std::vector<std::vector<CMenuItem*>> inItem)
{
	CMenu* menu = new CMenu;

	if (menu == nullptr)
	{
		return nullptr;
	}

	menu->Init();

	menu->m_pos = inPos;	// �ʒu�̐ݒ�
	menu->m_fream = inFream;	// �t���[���̐ݒ�

	if (menu->m_fream != nullptr)
	{
		menu->m_fream->SetPos((D3DXVECTOR3)menu->m_pos);	// �t���[���̈ʒu��ݒ�
		menu->m_Area = inArea;	// �͈͂̐ݒ�
		menu->m_fream->SetSize((D3DXVECTOR2)menu->m_Area);	// �t���[���̑傫����ݒ�
	}

	menu->m_item = inItem;	// ���ڂ̐ݒ�

	for (int i = 0; i < menu->m_item.size(); i++)
	{
		for (int j = 0; j < menu->m_item.at(i).size(); j++)
		{
			CMenuItem* item = menu->m_item[i][j];
			D3DXVECTOR2 pos = menu->m_pos;
			pos.y -= ((menu->m_item.size() - 1) * 0.5f) * (item->GetSize().y + menu->m_fInterval.y);
			pos.x -= ((menu->m_item.at(i).size() - 1) * 0.5f) * (item->GetSize().x + menu->m_fInterval.x);

			pos.y += i * (item->GetSize().y + menu->m_fInterval.y);
			pos.x += j * (item->GetSize().x + menu->m_fInterval.x);

			item->SetPos(D3DXVECTOR3(pos.x,pos.y,0.0f));
		}
	}

	return menu;
}

//=========================================
// 
// �t���[���N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CMenuFream::CMenuFream(CObject::TYPE type) :
	CObject2D(type)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CMenuFream::~CMenuFream()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CMenuFream::Init()
{
	CObject2D::Init();
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CMenuFream::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CMenuFream::Update()
{
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CMenuFream::Draw()
{
	CObject2D::Draw();
}

//=========================================
// 
// ���ڃN���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CMenuItem::CMenuItem(CObject::TYPE type) :
	CObject2D(type)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CMenuItem::~CMenuItem()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CMenuItem::Init()
{
	CObject2D::Init();
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CMenuItem::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CMenuItem::Update()
{
}

//-----------------------------------------
// �I����Ԃ̃A�b�v�f�[�g
//-----------------------------------------
void CMenuItem::SelectUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(3));
}

//-----------------------------------------
// �ʏ��Ԃ̃A�b�v�f�[�g
//-----------------------------------------
void CMenuItem::NomalUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CMenuItem::Draw()
{
	CObject2D::Draw();
}
