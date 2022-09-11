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
#include "ui_obtained_setnumber.h"
#include <assert.h>
#include <functional>

#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGame::CGame() :
	m_stage(nullptr),
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0)
{
	m_winNumber.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(m_countDownUI == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// �������J�E���g����int�^���`�[�������쐬����B
	m_winNumber.resize(2);
	for (int i = 0; i < m_winNumber.size(); i++)
	{
		m_winNumber[i] = 0;
	}

	m_needWinNumber = 5;
	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 0);
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
	if (m_countDownUI != nullptr)
	{
		m_countDownUI->Uninit();
		delete m_countDownUI;
		m_countDownUI = nullptr;
	}

	if (m_stage != nullptr)
	{
		m_stage->Uninit();
		delete m_stage;
		m_stage = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update()
{
	// ���E���h�I��UI�̍X�V
	if (m_obtainedSetNumberUI != nullptr)
	{
		m_obtainedSetNumberUI->Update();
		if (m_obtainedSetNumberUI->GetIsDeleted())
		{
			// ������x�Q�[�����s��
			ResetStage();

			// delete����
			m_obtainedSetNumberUI->Uninit();
			delete m_obtainedSetNumberUI;
			m_obtainedSetNumberUI = nullptr;
		}
		return;
	}

	// �J�E���g�_�E��UI�̍X�V
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
	}

	if (m_stage != nullptr)
	{
		m_stage->Update();
		if (m_stage->GetEndSet())
		{
			m_winnerIndex = m_stage->GetWinnerIndex();
			m_winNumber[m_winnerIndex]++;

			auto isSetCheack = [this](int idx)
			{
				return m_winNumber[idx] >= m_needWinNumber;
			};

			bool isRoundCountWon = false;
			for (int i = 0; i < m_winNumber.size(); i++)
			{
				if (isSetCheack(i))
				{
					isRoundCountWon = true;
				}
			}

			if (isRoundCountWon)
			{ // �w�萔���ǂ��炩�̃`�[�����������ꍇ
				//	��ʂ̑J��
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			else
			{
				// ������x���O��UI�ōÑ�
				if (m_obtainedSetNumberUI != nullptr)
				{
					// ������ʂ����i�K��UI�������ĂȂ������ꍇ�x��
					assert(false);
				}

				D3DXVECTOR2 pos = D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
				m_obtainedSetNumberUI = CObtainedSetNumberUI::Create(pos);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}

//-----------------------------------------------------------------------------
// �X�e�[�W�̃��Z�b�g
//-----------------------------------------------------------------------------
void CGame::ResetStage()
{
	m_stage->AllDelete();	// �X�e�[�W��ō�����I�u�W�F�N�g��S�ĉ��
	m_stage->Uninit();	// �I��
	delete m_stage;
	m_stage = nullptr;

	m_stage = new CStage;
	m_stage->Init();	// ������
	D3DXVECTOR2 pos = D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
	m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n
}
