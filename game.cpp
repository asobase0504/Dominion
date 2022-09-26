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
#include "ui_end_game.h"
#include "pause.h"
#include "menu.h"
#include "menu_stage_select.h"
#include "input.h"
#include <assert.h>
#include <functional>
#include "sound.h"

#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGame::CGame() :
	m_pause(nullptr),
	m_stageSelect(nullptr),
	m_stageIndex(0),
	m_stage(nullptr),
	m_peopleNumberSelect(nullptr),
	m_charcterSelect(nullptr),
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_endGameUI(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0),
	m_peopleNumber(0)
{
	m_stageInfo.clear();
	m_controllerIndex.clear();
	m_winNumber.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(m_stageSelect == nullptr);
	assert(m_endGameUI == nullptr);
	assert(m_stage == nullptr);
	assert(m_peopleNumberSelect == nullptr);
	assert(m_charcterSelect == nullptr);
	assert(m_countDownUI == nullptr);
	assert(m_obtainedSetNumberUI == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 0);
		bg->SetSize(CApplication::GetInstance()->GetScreenSize());
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}

	// �X�e�[�W���̎擾
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage01.json")["STAGE"];

	StageSelectInit();

	m_needWinNumber = 1;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �X�e�[�W�I�𒆏�����
//-----------------------------------------------------------------------------
void CGame::StageSelectInit()
{
	m_updateLagTime = 0;

	// ���X�e�[�W�̐ݒu
	ResetStage();

	m_stageSelect = CStageSelectMenu::Create(m_stageInfo.size());
}

//-----------------------------------------------------------------------------
// �l���I��
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectInit()
{
	m_updateLagTime = 0;

	// �t���[���̐ݒ�
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < 2; i++)
	{
		CMenuItem* item = new CMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(80.0f, 80.0f));					// �傫���̐ݒ�
		item->SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;
	m_peopleNumberSelect = CMenu::Create(CApplication::GetInstance()->GetScreenCenter(), area, fream);
	m_peopleNumberSelect->SetItems(items);
}

//-----------------------------------------------------------------------------
// �L�����N�^�[�I��
//-----------------------------------------------------------------------------
void CGame::CharcterSelectInit()
{
	m_updateLagTime = 0;

	// �t���[���̐ݒ�
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < m_peopleNumber; i++)
	{
		CMenuItem* item = new CMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(80.0f, 80.0f));					// �傫���̐ݒ�
		item->SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;
	m_charcterSelect = CMenu::Create(CApplication::GetInstance()->GetScreenCenter(), area, fream);
	m_charcterSelect->SetItems(items);

	// �R���g���[���[�̔ԍ����v���C���[�����쐬����
	m_controllerIndex.resize(m_peopleNumber);

	for (int i = 0; i < (int)m_controllerIndex.size(); i++)
	{
		m_controllerIndex[i] = -2;
	}
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

	if (m_obtainedSetNumberUI != nullptr)
	{
		m_obtainedSetNumberUI->Uninit();
		delete m_obtainedSetNumberUI;
		m_obtainedSetNumberUI = nullptr;
	}

	if (m_stage != nullptr)
	{
		m_stage->Uninit();
		delete m_stage;
		m_stage = nullptr;
	}

	if (m_stageSelect != nullptr)
	{
		m_stageSelect->Uninit();
		delete m_stageSelect;
		m_stageSelect = nullptr;
	}

	if (m_endGameUI != nullptr)
	{
		m_endGameUI->Uninit();
		delete m_endGameUI;
		m_endGameUI = nullptr;
	}

	if (m_peopleNumberSelect != nullptr)
	{
		m_peopleNumberSelect->Uninit();
		delete m_peopleNumberSelect;
		m_peopleNumberSelect = nullptr;
	}

	if (m_charcterSelect != nullptr)
	{
		m_charcterSelect->Uninit();
		delete m_charcterSelect;
		m_charcterSelect = nullptr;
	}

	if (m_pause != nullptr)
	{
		m_pause->Uninit();
		delete m_pause;
		m_pause = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update()
{
	// �X�e�[�W�I��
	StageSelectUpdate();
	if (m_stageSelect != nullptr)
	{
		if (CInput::GetKey()->Trigger(KEY_BACK))
		{
			m_stageSelect->Uninit();
			delete m_stageSelect;
			m_stageSelect = nullptr;

			CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
			// ��ʂ̑J��
			CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			return;
		}
	}

	// �l���I��
	PeopleNumberSelectUpdate();

	// �L�����N�^�[�I��
	CharctorSelect();

	if (m_charcterSelect == nullptr && m_peopleNumberSelect == nullptr && m_stageSelect == nullptr)
	{
		// �o�g����
		BattleUpdate();
	}
}

//-----------------------------------------------------------------------------
// �X�e�[�W�I�𒆍X�V
//-----------------------------------------------------------------------------
void CGame::StageSelectUpdate()
{
	if (m_stageSelect == nullptr)
	{
		return;
	}

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	m_stageSelect->Update();

	if (m_stageIndex != m_stageSelect->GetSelectIdx()[1])
	{
		m_stageIndex = m_stageSelect->GetSelectIdx()[1];

		ResetStage();	// ���X�e�[�W�̐ݒu
	}

	CInput* input = CInput::GetKey();
	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
		m_stageSelect->SetIsDeleted();
		m_stageSelect->Uninit();
		delete m_stageSelect;
		m_stageSelect = nullptr;

		if (m_peopleNumberSelect == nullptr)
		{
			PeopleNumberSelectInit();
		}
		else
		{
			assert(false);
		}
	}
}

//-----------------------------------------------------------------------------
// �l���I�𒆍X�V
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectUpdate()
{
	if (m_peopleNumberSelect == nullptr)
	{
		return;
	}

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	m_peopleNumberSelect->Update();

	CInput* input = CInput::GetKey();

	// ����{�^��
	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);

		// �l���̐ݒ�
		m_peopleNumber = m_peopleNumberSelect->GetSelectIdx()[1] * 2 + 2;

		// �I������
		m_peopleNumberSelect->SetIsDeleted();
		m_peopleNumberSelect->Uninit();
		delete m_peopleNumberSelect;
		m_peopleNumberSelect = nullptr;

		// �������J�E���g����int�^���`�[�������쐬����B
		m_winNumber.resize(m_peopleNumber);
		for (int i = 0; i < (int)m_winNumber.size(); i++)
		{
			m_winNumber[i] = 0;
		}

		if (m_charcterSelect == nullptr)
		{
			CharcterSelectInit();
			return;
		}
		else
		{
			assert(false);
		}
	}

	// �߂�{�^��
	if (input->Trigger(KEY_BACK))
	{
		if (m_stageSelect == nullptr)
		{
			m_peopleNumberSelect->SetIsDeleted();
			m_peopleNumberSelect->Uninit();
			delete m_peopleNumberSelect;
			m_peopleNumberSelect = nullptr;

			StageSelectInit();
			return;
		}
		else
		{
			assert(false);
		}
	}
}

//-----------------------------------------------------------------------------
// �L�����N�^�[�I�𒆍X�V
//-----------------------------------------------------------------------------
void CGame::CharctorSelect()
{
	if (m_charcterSelect == nullptr)
	{
		return;
	}

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	m_charcterSelect->Update();

	CInput* input = CInput::GetKey();
	std::vector<int> deviceIndexUp = input->TriggerDevice(KEY_UP);
	std::vector<int> deviceIndexDown = input->TriggerDevice(KEY_DOWN);

	for (int i = 0; i < (int)deviceIndexUp.size(); i++)
	{
		bool isDuplication = false;	// �d�����Ă��邩�ۂ�

		for (int j = 0; j < (int)m_controllerIndex.size(); j++)
		{
			/* ���d���`�F�b�N�� */

			if (m_controllerIndex[j] == deviceIndexUp[i])
			{
				isDuplication = true;
				break;
			}
		}

		if (isDuplication)
		{
			continue;
		}

			/* ���d�������������� */

			for (int j = 0; j < (int)m_controllerIndex.size(); j++)
			{
				if (m_controllerIndex[j] == -2)
				{
					m_controllerIndex[j] = deviceIndexUp[i];
					break;
				}
			}
	}

	// ����{�^��
	if (input->Trigger(KEY_DECISION))
	{
		m_stage->SetControllerIndex(m_controllerIndex);

		// �I������
		m_charcterSelect->SetIsDeleted();
		m_charcterSelect->Uninit();
		delete m_charcterSelect;
		m_charcterSelect = nullptr;

		// �J�E���g�_�E���̏�����
		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// �ʒu��ݒ�
		m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n
	}

	// �߂�{�^��
	if (input->Trigger(KEY_BACK))
	{
		if (m_stageSelect == nullptr)
		{
			m_charcterSelect->SetIsDeleted();
			m_charcterSelect->Uninit();
			delete m_charcterSelect;
			m_charcterSelect = nullptr;

			PeopleNumberSelectInit();
			return;
		}
		else
		{
			assert(false);
		}
	}
}

//-----------------------------------------------------------------------------
// �o�g�����X�V
//-----------------------------------------------------------------------------
void CGame::BattleUpdate()
{
	if (m_pause != nullptr)
	{
		m_pause->Update();
		if (m_pause->GetIsDeleted())
		{
			CPause::Status status = m_pause->GetStatus();

			m_pause->Uninit();
			delete m_pause;
			m_pause = nullptr;

			switch (status)
			{
			case CPause::RESTART:
			{
				for (int i = 0; i < (int)m_winNumber.size(); i++)
				{
					m_winNumber[i] = 0;
				}
				// ������x�Q�[�����s��
				ResetStage();
				// �J�E���g�_�E���̏�����
				D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// �ʒu��ݒ�
				m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n
			}
				break;
			case CPause::END:
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
				break;
			default:
				break;
			}
		}
		return;
	}

	if (m_endGameUI != nullptr)
	{
		m_endGameUI->Update();
		if (m_endGameUI->GetIsDeleted())
		{
			std::vector<int> index = m_endGameUI->GetMenu()->GetSelectIdx();

			// delete����
			m_endGameUI->Uninit();
			delete m_endGameUI;
			m_endGameUI = nullptr;

			if (index[0] == 1)
			{
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			if (index[0] == 0)
			{
				for (int i = 0; i < (int)m_winNumber.size(); i++)
				{
					m_winNumber[i] = 0;
				}
				// ������x�Q�[�����s��
				ResetStage();
				// �J�E���g�_�E���̏�����
				D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// �ʒu��ݒ�
				m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n
			}
		}
		return;
	}

	// ���E���h�I��UI�̍X�V
	if (m_obtainedSetNumberUI != nullptr)
	{
		m_obtainedSetNumberUI->Update();
		if (m_obtainedSetNumberUI->GetIsDeleted())
		{
			// ������x�Q�[�����s��
			ResetStage();
			// �J�E���g�_�E���̏�����
			D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// �ʒu��ݒ�
			m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n

			// delete����
			m_obtainedSetNumberUI->Uninit();
			delete m_obtainedSetNumberUI;
			m_obtainedSetNumberUI = nullptr;
		}
		return;
	}

	/* �����E���h�I��UI�����݂��Ȃ��ꍇ�� */

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
		return;
	}

	/* �����E���h�J�n�̃J�E���g�_�E��UI�����݂��Ȃ��ꍇ�� */

	if (m_stage == nullptr)
	{
		return;
	}

	/* ��Stage����������Ă�ꍇ�� */

	// �퓬��
	m_stage->Update();
	
	if (CInput::GetKey()->Trigger(KEY_PAUSE) && m_countDownUI == nullptr)
	{
		if (m_pause == nullptr)
		{
			m_pause = new CPause;
			m_pause->Init();
		}
	}

	if (m_stage->GetEndSet())
	{ // �퓬�I����
		BattleEnd();
	}
}

//-----------------------------------------------------------------------------
// �o�g���I�����X�V
//-----------------------------------------------------------------------------
void CGame::BattleEnd()
{
	m_winnerIndex = m_stage->GetWinnerIndex();
	m_winNumber[m_winnerIndex]++;

	bool isRoundCountWon = false;
	for (int i = 0; i < (int)m_winNumber.size(); i++)
	{
		if (m_winNumber[i] >= m_needWinNumber)
		{
			isRoundCountWon = true;
		}
	}

	if (isRoundCountWon)
	{ // �w�萔���ǂ��炩�̃`�[�����������ꍇ
		// �G���h�Q�[��UI�̕\��
		assert(m_endGameUI == nullptr);	// ������ʂ����i�K��UI�����݂��Ă����ꍇ�x��
		m_endGameUI = CEndGameUI::Create(CApplication::GetInstance()->GetScreenCenter());
	}
	else
	{
		// ���E���h�I��UI�̕\��
		assert(m_obtainedSetNumberUI == nullptr);	// ������ʂ����i�K��UI�����݂��Ă����ꍇ�x��
		m_obtainedSetNumberUI = CObtainedSetNumberUI::Create(CApplication::GetInstance()->GetScreenCenter());
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
	if (m_stage != nullptr)
	{
		m_stage->AllDelete();	// �X�e�[�W��ō�����I�u�W�F�N�g��S�ĉ��
		m_stage->Uninit();	// �I��
		delete m_stage;
		m_stage = nullptr;
	}

	m_stage = new CStage;
	m_stage->Init(GetStageInfo());	// ������
}
