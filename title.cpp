//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "title.h"
#include <assert.h>

#include "application.h"
#include "object2d.h"
#include "menu.h"
#include "input.h"
#include "menu_item.h"
#include "sound.h"
#include "tutorial.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTitle::CTitle() : 
	m_manu(nullptr),
	m_status(Status::NONE)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTitle::~CTitle()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CTitle::Init()
{
	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create();
		bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}


	// �^�C�g��2�̐ݒ�
	{
		CObject2D* title = CObject2D::Create();
		title->SetSize(D3DXVECTOR2(CApplication::GetInstance()->CENTER_X * 1.2f, CApplication::GetInstance()->CENTER_Y * 0.5f));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X * 0.65f, CApplication::GetInstance()->CENTER_Y * 0.25f, 0.0f);	// �ʒu�̎擾
		title->SetTexture("TEXT_TITLE2");
		title->SetPos(pos);
		title->SetColor(CApplication::GetInstance()->GetColor(1));
	}
	// �^�C�g��1�̐ݒ�
	{
		CObject2D* title = CObject2D::Create();
		title->SetSize(D3DXVECTOR2(CApplication::GetInstance()->CENTER_X * 1.2f, CApplication::GetInstance()->CENTER_Y * 0.5f));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X * 0.65f, CApplication::GetInstance()->CENTER_Y * 0.25f, 0.0f);	// �ʒu�̎擾
		title->SetTexture("TEXT_TITLE1");
		title->SetPos(pos);
		title->SetColor(CApplication::GetInstance()->GetColor(0));
	}

	// ���j���[�̐ݒ�
	{
		// �t���[���̐ݒ�
		CMenuFream* fream = new CMenuFream;
		{
			fream->Init();
			fream->SetColor(CApplication::GetInstance()->GetColor(1));
		}

		// ���ڂ̐ݒ�
		std::vector<std::vector<CMenuItem*>> items;
		for (int i = 0; i < (int)CTitle::Status::MAX; i++)
		{
			std::vector<CMenuItem*> X;

			CMenuItem* item = new CPauseMenuItem;
			item->Init();
			item->SetSize(D3DXVECTOR2(465.0f, 80.0f));			// �傫���̐ݒ�
			item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

			switch ((CTitle::Status)i)
			{
			case CTitle::Status::GAME_STAET:
				item->SetTexture("TEXT_START");
				break;
			case CTitle::Status::TUTORIAL:
				item->SetTexture("TEXT_TUTORIAL");
				break;
			case CTitle::Status::CUSTOMIZE:
				item->SetTexture("TEXT_CUSTOM");
				break;
			case CTitle::Status::OPSITON:
				item->SetTexture("TEXT_OPTION");
				break;
			default:
				assert(false);
				break;
			}

			X.push_back(item);
			items.push_back(X);
		}

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X * 1.55f, CApplication::GetInstance()->CENTER_Y * 1.2f);
		D3DXVECTOR2 area(500.0f, 550.0f);
		m_manu = CMenu::Create(pos, area, fream);
		m_manu->SetItems(items);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTitle::Uninit()
{
	if (m_manu != nullptr)
	{
		m_manu->Uninit();
		delete m_manu;
		m_manu = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CTitle::Update()
{
	CInput* input = CInput::GetKey();
	switch (m_status)
	{
	case CTitle::Status::NONE:
	{
		if (m_manu != nullptr)
		{
			m_manu->Update();
		}

		if (input->Trigger(KEY_DECISION))
		{
			m_decisionDvice = input->TriggerDevice(KEY_DECISION).at(0);
			m_status = (CTitle::Status)m_manu->GetSelectIdx()[0];
		}

		if (m_manu->Decision(input->Trigger(KEY_DECISION)))
		{
			m_status = (CTitle::Status)m_manu->GetSelectIdx()[0];
		}
	}
		break;
	case CTitle::Status::GAME_STAET:
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::GAME);
		break;
	case CTitle::Status::TUTORIAL:
	{
		int decisionDvice = m_decisionDvice;
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TUTORIAL);
		CTutorial* tutorialMode = (CTutorial*)CApplication::GetInstance()->GetMode();
		tutorialMode->SetControllerIndex(decisionDvice);
	}
		break;
	case CTitle::Status::CUSTOMIZE:
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::CUSTUM);
		break;
	case CTitle::Status::OPSITON:
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::GAME);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CTitle::Draw()
{
}
