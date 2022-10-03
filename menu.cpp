//=============================================================================
// 
// ���j���[�N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "menu.h"
#include "menu_item.h"
#include "application.h"
#include "sound.h"
#include "input.h"

//-----------------------------------------------------------------------------
// ��`
//-----------------------------------------------------------------------------
const int CMenu::DECISION_AFTERGLOW_TIME = 30;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CMenu::~CMenu()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CMenu::Init()
{
	m_fInterval = { 5.0f,25.0f };
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CMenu::Uninit()
{
	m_item.clear();
	m_item.clear();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ����O�̍X�V
//-----------------------------------------------------------------------------
void CMenu::UpdateBeforeDecision()
{
	CInput* input = CInput::GetKey();
	if (input->Trigger(KEY_UP))
	{
		Select(CMenu::TOP);
	}
	if (input->Trigger(KEY_DOWN))
	{
		Select(CMenu::DOWN);
	}
	if (input->Trigger(KEY_LEFT))
	{
		Select(CMenu::LEFT);
	}
	if (input->Trigger(KEY_RIGHT))
	{
		Select(CMenu::RIGHT);
	}

	for (int i = 0; i < (int)m_item.size(); i++)
	{
		for (int j = 0; j < (int)m_item[i].size(); j++)
		{
			if (m_item[i][j]->GetisPopNow())
			{
				m_item[i][j]->PopUpdate();
			}
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

//-----------------------------------------------------------------------------
// �����̍X�V
//-----------------------------------------------------------------------------
void CMenu::UpdateAfterDecision()
{
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CMenu::Draw()
{
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CMenu::Select(SELECT_DIRECTION inDirection)
{
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT);
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

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
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
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
		m_isDexision = false;
	}
	return check;
}

//-----------------------------------------------------------------------------
// �I�����ꂽ�ԍ��̒ǉ�X��
//-----------------------------------------------------------------------------
void CMenu::AddItemX(CMenuItem * inItem, int indexY)
{
	m_item[indexY].push_back(inItem);
	SetItemPos();
}

//-----------------------------------------------------------------------------
// �I�����ꂽ�ԍ��̒ǉ�Y��
//-----------------------------------------------------------------------------
void CMenu::AddItemY(std::vector<CMenuItem*> inItems)
{
	m_item.push_back(inItems);
	SetItemPos();
}

//-----------------------------------------------------------------------------
// �I�����ꂽ�ԍ��̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetSelectIdx(int Y, int X)
{
	m_selectIdx[0] = Y;
	m_selectIdx[1] = X;
}

//-----------------------------------------------------------------------------
// �I�����ꂽ�ԍ��̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetSelectIdx(std::vector<int> inIdx)
{
	m_selectIdx = inIdx;
}

//-----------------------------------------------------------------------------
// ���ڊԊu�̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetInterval(D3DXVECTOR2 inIntervar)
{
	m_fInterval = inIntervar;
	SetItemPos();
}

//-----------------------------------------------------------------------------
// ���ڊԊu�̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetInterval(float inIntervarX, float inIntervarY)
{
	m_fInterval.x = inIntervarX;
	m_fInterval.y = inIntervarY;
	SetItemPos();
}

//-----------------------------------------------------------------------------
// ����\��̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetIsDeleted()
{
	for (int i = 0; i < (int)m_item.size(); i++)
	{
		for (int j = 0; j < (int)m_item[i].size(); j++)
		{
			m_item[i][j]->SetIsDeleted(true);
		}
	}
	m_fream->SetIsDeleted(true);
}

//-----------------------------------------------------------------------------
// �A�C�e���̐ݒ�
//-----------------------------------------------------------------------------
void CMenu::SetItems(const std::vector<std::vector<CMenuItem*>>& inItems)
{
	m_item = inItems;	// ���ڂ̐ݒ�
	SetItemPos();
}

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CMenu * CMenu::Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea, CMenuFream * inFream)
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

	return menu;
}

//-----------------------------------------------------------------------------
// �A�C�e���̈ʒu��ݒ肷��
//-----------------------------------------------------------------------------
void CMenu::SetItemPos()
{
	for (int i = 0; i < (int)m_item.size(); i++)
	{
		for (int j = 0; j < (int)m_item.at(i).size(); j++)
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

//=============================================================================
// 
// �t���[���N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CMenuFream::CMenuFream(CObject::TYPE type) :
	CObject2D(type)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CMenuFream::~CMenuFream()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CMenuFream::Init()
{
	CObject2D::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CMenuFream::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CMenuFream::Update()
{
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CMenuFream::Draw()
{
	CObject2D::Draw();
}

//=============================================================================
// 
// ���ڊ�{�N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CMenuItem::CMenuItem(CObject::TYPE type) :
	CObject2D(type,4),
	m_isPopNow(false),
	m_isPopDuringUpdateAccept(false)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CMenuItem::~CMenuItem()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CMenuItem::Init()
{
	CObject2D::Init();
	m_isPopNow = true;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �o����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CMenuItem::PopUpdate()
{
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CMenuItem::SelectUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------------------------------------------
// �ʏ��Ԃ̍X�V
//-----------------------------------------------------------------------------
void CMenuItem::NomalUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(2));
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CMenuItem::EndUpdate()
{
}
