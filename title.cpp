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
#include "input_keybord.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTitle::CTitle()
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
	CApplication::GetInstance()->SetThemeColor(1);

	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create();
		bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
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

			CMenuItem* item = new CMenuItem;
			item->Init();
			item->SetSize(D3DXVECTOR2(465.0f, 80.0f));			// �傫���̐ݒ�
			item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

			switch ((CTitle::Status)i)
			{
			case CTitle::Status::GAME_STAET:
				item->SetTexture("TEXT_GAMESTART");
				break;
			case CTitle::Status::TUTORIAL:
				item->SetTexture("TEXT_TUTORIAL");
				break;
			case CTitle::Status::CUSTOMIZE:
				item->SetTexture("TEXT_CUSTOMIZE");
				break;
			case CTitle::Status::OPSITON:
				item->SetTexture("TEXT_OPSITON");
				break;
			case CTitle::Status::EXIT:
				item->SetTexture("TEXT_EXIT");
				break;
			default:
				assert(false);
				break;
			}

			X.push_back(item);
			items.push_back(X);
		}

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
		D3DXVECTOR2 area(500.0f, 550.0f);
		m_manu = CMenu::Create(pos, area, fream, items);
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
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	if (m_manu != nullptr)
	{
		m_manu->Update();
		if (input->GetTrigger(DIK_W))
		{
			m_manu->Select(CMenu::TOP);
		}
		if (input->GetTrigger(DIK_S))
		{
			m_manu->Select(CMenu::DOWN);
		}
		if (input->GetTrigger(DIK_A))
		{
			m_manu->Select(CMenu::LEFT);
		}
		if (input->GetTrigger(DIK_D))
		{
			m_manu->Select(CMenu::RIGHT);
		}
	}

	if (input->GetTrigger(DIK_RETURN) && (m_manu->GetSelectIdx()[0] == 0))
	{
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::GAME);
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CTitle::Draw()
{
}
