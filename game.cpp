//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
#include "stage.h"
#include "ui_countdown.h"
#include <assert.h>
#include <functional>

#include "application.h"
#include "input.h"
#include "input_keybord.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGame::CGame() : 
	m_stage(nullptr),
	m_setNumber(0)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 1);
		bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}

	m_stage = new CStage;

	m_stage->Init();

	m_countDownUI = CCountDownUI::Create(D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y));
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	if (m_countDownUI != nullptr)
	{
		m_countDownUI->Update();
		if (m_countDownUI->GetIsDeleted())
		{
			// delete����
			m_countDownUI->Uninit();
			delete m_countDownUI;
			m_countDownUI = nullptr;
		}
		return;
	}

	if (m_stage != nullptr)
	{
		m_stage->Update();
		if (m_stage->GetEndSet())
		{
			m_setNumber++;
			if ((m_setNumber <= 5))
			{
				m_stage->Uninit();
				delete m_stage;
				m_stage = nullptr;

				m_stage = new CStage;
				m_stage->Init();
				m_countDownUI = CCountDownUI::Create(D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y));
			}
			else
			{
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
		}
	}

	//	��ʂ̑J��
	if (input->GetTrigger(DIK_RETURN))
	{
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
		return;
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}
