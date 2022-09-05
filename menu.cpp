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
// ��`
//-----------------------------------------
const int CMenu::DECISION_AFTERGLOW_TIME = 30;

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
	m_AroundWhitespace(D3DXVECTOR2(0.0f, 0.0f)),
	m_isDexision(false),
	m_decisionAfterglowCount(0)
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
	if (!m_isDexision)
	{
		UpdateBeforeDecision();
	}
	else
	{
		UpdateAfterDecision();
	}
}

//-----------------------------------------
// ����O�̍X�V
//-----------------------------------------
void CMenu::UpdateBeforeDecision()
{
	for (int i = 0; i < m_item.size(); i++)
	{
		for (int j = 0; j < m_item[i].size(); j++)
		{
			if (m_selectIdx[0] == i && m_selectIdx[1] == j)
			{
				// ���g���I�𒆂̏ꍇ
				m_item[i][j]->SelectUpdate();
			}
			else
			{
				// ���g���I�𒆂ł͂Ȃ��ꍇ
				m_item[i][j]->NomalUpdate();
			}
		}
	}
}

//-----------------------------------------
// �����̍X�V
//-----------------------------------------
void CMenu::UpdateAfterDecision()
{
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
// ����
//-----------------------------------------
bool CMenu::Decision(bool inDecison)
{
	// ���肵�����ۂ�
	if (!m_isDexision)
	{
		m_isDexision = inDecison;
		return false;
	}

	/* �����肵�Ă���ꍇ�� */

	m_decisionAfterglowCount++;

	bool check = m_decisionAfterglowCount >= DECISION_AFTERGLOW_TIME;
	if (check)
	{
		m_isDexision = false;
	}
	return check;
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

	menu->SetItemPos();

	return menu;
}

//-----------------------------------------
// �A�C�e���̈ʒu��ݒ肷��
//-----------------------------------------
void CMenu::SetItemPos()
{
	for (int i = 0; i < m_item.size(); i++)
	{
		for (int j = 0; j < m_item.at(i).size(); j++)
		{
			CMenuItem* item = m_item[i][j];
			D3DXVECTOR2 pos = m_pos;
			pos.y -= ((m_item.size() - 1) * 0.5f) * (item->GetSize().y + m_fInterval.y);
			pos.x -= ((m_item.at(i).size() - 1) * 0.5f) * (item->GetSize().x + m_fInterval.x);

			pos.y += i * (item->GetSize().y + m_fInterval.y);
			pos.x += j * (item->GetSize().x + m_fInterval.x);

			item->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
		}
	}

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
