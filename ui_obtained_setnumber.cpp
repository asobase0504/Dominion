//=============================================================================
//
// ���E���h�擾����\������N���X
// Author YudaKaito
//
//=============================================================================
#include "ui_obtained_setnumber.h"
#include "application.h"
#include "game.h"

//-----------------------------------------------------------------------------
// ��`
//-----------------------------------------------------------------------------
const int CObtainedSetNumberUI::PRIORITY_BELONG = 5;	// �v���C�I���e�B
const int CObtainedSetNumberUI::NEW_SCORE_END_IN_TIME = 60;		// �V�K���_�̏o�����I��鎞��
const int CObtainedSetNumberUI::NEW_SCORE_BEGIN_OUT_TIME = 120;	// �V�K���_�̏������n�܂鎞��
const int CObtainedSetNumberUI::NEW_SCORE_END_OUT_TIME = 160;	// �V�K���_�̏������I��鎞��
const int CObtainedSetNumberUI::NEXT_STAGE_TIME = 160
;	// ���̃X�e�[�W�ֈڍs����

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CObtainedSetNumberUI * CObtainedSetNumberUI::Create(const D3DXVECTOR2 & inPos)
{
	CObtainedSetNumberUI* obtaindeSetNumber = new CObtainedSetNumberUI;

	obtaindeSetNumber->Init();

	// �t���[���̏�����
	{
		CObject2D*& fream = obtaindeSetNumber->m_fream;
		fream = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);
		if (FAILED(fream->Init()))
		{
			return nullptr;
		}
		fream->SetPos((D3DXVECTOR3)inPos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(0);
		color.a = 0.95f;
		fream->SetColor(color);
		fream->SetSize({ (float)CApplication::GetInstance()->SCREEN_WIDTH,100.0f });
		fream->SetShouldStopAlsoUpdate();
	}

	int teem = 2;
	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();

	// ���E���h���̎擾
	{
		std::vector<std::vector<CObject2D*>>& setCountUI = obtaindeSetNumber->m_setCountUI;	// �Z���ϐ��ɕύX

		setCountUI.resize(teem);	// �`�[���̐����傫�����m��

		int needWinNumber = modeGame->GetNeedWinNumber();

		for (int i = 0; i < (int)setCountUI.size(); i++)
		{
			setCountUI[i].resize(needWinNumber);

			for (int j = 0; j < (int)setCountUI[i].size(); j++)
			{
				setCountUI[i][j] = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);

				// ������
				if (FAILED(setCountUI[i][j]->Init()))
				{
					return nullptr;
				}

				/* ���Q�ڈڍs�̃I�u�W�F�N�g�������ꍇ�� */

				D3DXVECTOR3 pos = (D3DXVECTOR3)inPos;
				pos.x = 0.0f;
				float center = (float)CApplication::GetInstance()->SCREEN_WIDTH / (float)teem;
				pos.x += center * i + center * 0.5f + ((j - setCountUI[i].size() * 0.5f) * 50.0f);
				setCountUI[i][j]->SetPos(pos);
				D3DXCOLOR color = CApplication::GetInstance()->GetColor(1);
				setCountUI[i][j]->SetColor(color);
				setCountUI[i][j]->SetSize({ 30.0f,30.0f });
				setCountUI[i][j]->SetShouldStopAlsoUpdate();
			}
		}
	}

	// �擾�������E���h���̎擾
	{
		std::vector<std::vector<CObject2D*>>& tookSetUI = obtaindeSetNumber->m_tookSetUI;	// �Z���ϐ��ɕύX

		tookSetUI.resize(teem);	// �`�[���̐����傫�����m��
		
		for (int i = 0; i < (int)tookSetUI.size(); i++)
		{
			tookSetUI[i].resize(modeGame->GetWinNumber(i));

			int size = (int)tookSetUI[i].size();

			for (int j = 0; j < size; j++)
			{
				tookSetUI[i][j] = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);

				// ������
				if (FAILED(tookSetUI[i][j]->Init()))
				{
					return nullptr;
				}

				D3DXVECTOR3 pos = obtaindeSetNumber->m_setCountUI[i][j]->GetPos();
				tookSetUI[i][j]->SetPos(pos);
				D3DXCOLOR color = CApplication::GetInstance()->GetColor(3);
				tookSetUI[i][j]->SetColor(color);
				if ((modeGame->GetWinner() == i) && ((int)tookSetUI[i].size() == j))
				{ // ���҂̏ꍇ
					tookSetUI[i][j]->SetSize({ 0.0f,0.0f });
				}
				else
				{
					tookSetUI[i][j]->SetSize({ 25.0f,25.0f });
				}

				tookSetUI[i][j]->SetShouldStopAlsoUpdate();
			}
		}
	}

	return obtaindeSetNumber;
}

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CObtainedSetNumberUI::CObtainedSetNumberUI() :
	m_time(0),
	m_pos(D3DXVECTOR2(0.0f,0.0f)),
	m_isDeleted(false)
{
	m_tookSetUI.clear();
	m_setCountUI.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CObtainedSetNumberUI::~CObtainedSetNumberUI()
{
	assert(m_tookSetUI.empty());
	assert(m_setCountUI.empty());
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CObtainedSetNumberUI::Init()
{
	return E_NOTIMPL;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CObtainedSetNumberUI::Uninit()
{
	m_tookSetUI.clear();
	m_setCountUI.clear();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CObtainedSetNumberUI::Update()
{
	m_time++;
	if (m_time <= NEW_SCORE_END_IN_TIME)
	{
		CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
		CObject2D* point = m_tookSetUI[modeGame->GetWinner()][m_tookSetUI[modeGame->GetWinner()].size() - 1];
		float transitionSize = 25.0f / NEW_SCORE_END_IN_TIME;

		point->SetSize(D3DXVECTOR2(transitionSize * m_time, transitionSize * m_time));
	}

	if (m_time >= NEW_SCORE_BEGIN_OUT_TIME && m_time <= NEW_SCORE_END_OUT_TIME)
	{
		float count = (float)(m_time - NEW_SCORE_BEGIN_OUT_TIME) / (float)(NEW_SCORE_END_OUT_TIME - NEW_SCORE_BEGIN_OUT_TIME);
		float pow = count * count;
		//float pow = sinf((count * D3DX_PI) * 0.5f);

		for (int i = 0; i < (int)m_setCountUI.size(); i++)
		{
			for (int j = 0; j < (int)m_setCountUI[i].size(); j++)
			{
				D3DXVECTOR2 size = m_setCountUI[i][j]->GetSize() - m_setCountUI[i][j]->GetSize() * pow;
				m_setCountUI[i][j]->SetSize(size);
			}
		}

		for (int i = 0; i < (int)m_tookSetUI.size(); i++)
		{
			for (int j = 0; j < (int)m_tookSetUI[i].size(); j++)
			{
				D3DXVECTOR2 size = m_tookSetUI[i][j]->GetSize() - m_tookSetUI[i][j]->GetSize() * pow;
				m_tookSetUI[i][j]->SetSize(size);
			}
		}
	}

	if (m_time >= NEXT_STAGE_TIME)
	{
		SetIsDeleted();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CObtainedSetNumberUI::Draw()
{
}

//-----------------------------------------------------------------------------
// ����\��̐ݒ�
//-----------------------------------------------------------------------------
void CObtainedSetNumberUI::SetIsDeleted()
{
	m_isDeleted = true;
	for (int i = 0; i < (int)m_setCountUI.size(); i++)
	{
		for (int j = 0; j < (int)m_setCountUI[i].size(); j++)
		{
			m_setCountUI[i][j]->SetIsDeleted(true);
		}
	}

	for (int i = 0; i < (int)m_tookSetUI.size(); i++)
	{
		for (int j = 0; j < (int)m_tookSetUI[i].size(); j++)
		{
			m_tookSetUI[i][j]->SetIsDeleted(true);
		}
	}
	m_fream->SetIsDeleted(true);
}
