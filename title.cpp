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

		for (int i = 0; i < 4; i++)
		{
			std::vector<CMenuItem*> X;
			{
				if (i != 3)
				{
					for (int j = 0; j < 4; j++)
					{
						CMenuItem* item = new CMenuItem;
						item->Init();
						item->SetSize(D3DXVECTOR2(100.0f, 100.0f));
						item->SetColor(CApplication::GetInstance()->GetColor(0));

						X.push_back(item);
					}
				}
				else
				{
					for (int j = 0; j < 1; j++)
					{
						CMenuItem* item = new CMenuItem;
						item->Init();
						item->SetSize(D3DXVECTOR2(150.0f, 80.0f));
						item->SetColor(CApplication::GetInstance()->GetColor(0));

						X.push_back(item);
					}
				}
			}
			items.push_back(X);
		}

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
		D3DXVECTOR2 area(500.0f, 500.0f);
		m_manu = CMenu::Create(pos, area, fream, items);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTitle::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CTitle::Update()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	if (input->GetTrigger(DIK_RETURN))
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
