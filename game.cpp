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
#include "menu_number_Select.h"
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
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_endGameUI(nullptr),
	m_contrellerTextureBg(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0),
	m_peopleNumber(0)
{
	m_charcterBg.clear();
	m_charcter.clear();
	m_bulletSample.clear();
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
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage01.json").at("STAGE");

	StageSelectInit();

	m_needWinNumber = 3;
	m_peopleNumber = 2;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �X�e�[�W�I�𒆏�����
//-----------------------------------------------------------------------------
void CGame::StageSelectInit()
{
	// �A�b�v�f�[�g��ʂ����O�����Z�b�g
	m_updateLagTime = 0;

	// ���X�e�[�W�̐ݒu
	ResetStage();

	// �X�e�[�W�Z���N�g���j���[�̍쐬
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
		CNumberSelectMenuItem* item = new CNumberSelectMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(80.0f, 80.0f));					// �傫���̐ݒ�
		item->SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
		item->SetAbovePasteTexture(i == 0 ? "2PL" : "4PL");	// �A�C�e���̏�ɓ\��I�u�W�F�N�g�̃e�N�X�`���ݒ�
		X.push_back(item);
	}
	items.push_back(X);

	{
		D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
		area.y *= 0.25f;
		D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
		pos.y *= 1.5f;
		m_peopleNumberSelect = CMenu::Create(pos, area, fream);
		m_peopleNumberSelect->SetInterval({ 25.0f,25.0f });
		m_peopleNumberSelect->SetItems(items);
	}

	m_charcterBg.resize(2);
	for (int i = 0; i < 2; i++)
	{
		m_charcterBg.at(i) = new CObject2D(CObject::TYPE::NONE,2);
		m_charcterBg.at(i)->Init();

		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.x *= (0.5f + 1.0f * i);
		pos.y *= 0.75f;
		m_charcterBg.at(i)->SetPos(pos);	// �F�̐ݒ�
		m_charcterBg.at(i)->SetColor(CApplication::GetInstance()->GetColor(i == 0 ? 0 : 1));	// �F�̐ݒ�
		D3DXVECTOR2 size(CApplication::GetInstance()->GetScreenSize());
		size.x *= 0.5f;
		size.y *= 0.25f;
		m_charcterBg.at(i)->SetSize(size);	// �F�̐ݒ�
		m_charcterBg.at(i)->SetColorAlpha(0.95f);	// �F�̐ݒ�
	}

	ResetCharcterSample();
}

//-----------------------------------------------------------------------------
// �l���I���̍폜
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectDelete()
{
	for (int i = 0; i < (int)m_charcterBg.size(); i++)
	{
		m_charcterBg.at(i)->SetIsDeleted(true);
	}
	m_charcterBg.clear();

	for (int i = 0; i < (int)m_charcter.size(); i++)
	{
		m_charcter.at(i)->SetIsDeleted(true);
	}
	m_charcter.clear();

	for (int i = 0; i < (int)m_bulletSample.size(); i++)
	{
		m_bulletSample.at(i)->SetIsDeleted(true);
		m_bulletSample.at(i)->Uninit();
	}
	m_bulletSample.clear();
}

//-----------------------------------------------------------------------------
// �L�����N�^�[sample
//-----------------------------------------------------------------------------
void CGame::ResetCharcterSample()
{
	for (int i = 0; i < (int)m_charcter.size(); i++)
	{
		m_charcter.at(i)->SetIsDeleted(true);
	}
	m_charcter.clear();

	for (int i = 0; i < (int)m_bulletSample.size(); i++)
	{
		m_bulletSample.at(i)->SetIsDeleted(true);
	}
	m_bulletSample.clear();

	m_charcter.resize(m_peopleNumber);
	m_bulletSample.resize(m_peopleNumber);
	for (int i = 0; i < m_peopleNumber; i++)
	{
		bool team = (m_peopleNumber * 0.5f) <= i;

		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X / m_peopleNumber, CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.y *= 0.75f;
		pos.x += pos.x * (i * 2);
		D3DXVECTOR2 size(60.0f, 60.0f);

		m_charcter.at(i) = new CObject2D;
		m_charcter.at(i)->Init();		// ������
		m_charcter.at(i)->SetPos(pos);	// �ʒu�̐ݒ�
		m_charcter.at(i)->SetColor(CApplication::GetInstance()->GetColor(team ? 0 : 1));	// �F�̐ݒ�
		m_charcter.at(i)->SetSize(size);	// �傫���̐ݒ�
		m_charcter.at(i)->SetTexture("PLAYER");	// �e�N�X�`���̐ݒ�

		m_bulletSample.at(i) = new CObject2D;
		m_bulletSample.at(i)->Init();		// ������
		m_bulletSample.at(i)->SetPos(pos);	// �ʒu�̐ݒ�
		m_bulletSample.at(i)->SetColor(CApplication::GetInstance()->GetColor(team ? 1 : 0));	// �F�̐ݒ�
		m_bulletSample.at(i)->SetSize(size);	// �傫���̐ݒ�
		m_bulletSample.at(i)->SetTexture("BULLET_SAMPLE");	// �e�N�X�`���̐ݒ�
	}
}

//-----------------------------------------------------------------------------
// �L�����N�^�[�I��
//-----------------------------------------------------------------------------
void CGame::CharcterSelectInit()
{
	m_updateLagTime = 0;

	// �w�i�̐ݒ�
	if (m_contrellerTextureBg == nullptr)
	{
		m_contrellerTextureBg = new CObject2D;
		m_contrellerTextureBg->Init();
		m_contrellerTextureBg->SetColor(CApplication::GetInstance()->GetColor(0));
		m_contrellerTextureBg->SetColorAlpha(0.55f);
		D3DXVECTOR2 size = CApplication::GetInstance()->GetScreenSize();
		size.y *= 0.25f;
		m_contrellerTextureBg->SetSize(size);
		D3DXVECTOR3 pos(CApplication::GetInstance()->GetScreenCenter());
		pos.y *= 1.5f;
		pos.z = 0.0f;
		m_contrellerTextureBg->SetPos(pos);
	}

	// �R���g���[���[�U���̃e�N�X�`��
	m_contrellerTexture.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	m_contrellerTeam.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	m_contrellerPos.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
	{
		m_contrellerTeam.at(i) = 0;
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X / m_contrellerTexture.size(), CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.y *= 1.5f;
		pos.x += pos.x * (i * 2);
		D3DXVECTOR2 size(70.0f, 70.0f);

		m_contrellerTexture.at(i) = new CObject2D;
		m_contrellerTexture.at(i)->Init();		// ������
		m_contrellerPos.at(i) = pos;
		m_contrellerTexture.at(i)->SetPos(pos);	// �ʒu�̐ݒ�
		m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
		m_contrellerTexture.at(i)->SetSize(size);	// �傫���̐ݒ�

		if (CInput::GetKey()->GetAcceptJoyPadCount() != i)
		{
			m_contrellerTexture.at(i)->SetTexture("CONTLLER");	// �e�N�X�`���̐ݒ�
		}
		else
		{
			m_contrellerTexture.at(i)->SetTexture("KEYBOAD");	// �e�N�X�`���̐ݒ�
		}
	}

	// �R���g���[���[�̔ԍ����v���C���[�����쐬����
	m_controllerIndex.resize(m_peopleNumber);

	for (int i = 0; i < (int)m_controllerIndex.size(); i++)
	{
		m_controllerIndex.at(i) = -2;
	}
}

//-----------------------------------------------------------------------------
// �R���g���[���[�̔ԍ���ݒ�
//-----------------------------------------------------------------------------
void CGame::SetControllerIndex()
{
	int team1 = 0;
	int team2 = 1;
	for (int i = 0; i < (int)m_contrellerTeam.size(); i++)
	{
		if (m_contrellerTeam.at(i) == 0)
		{
			continue;
		}
		else if (m_contrellerTeam.at(i) == -1)
		{
			if (i == (int)m_contrellerTeam.size() - 1)
			{
				m_controllerIndex.at(team1) = -1;
			}
			else
			{
				m_controllerIndex.at(team1) = i;
			}
			team1 += 2;
		}
		else if (m_contrellerTeam.at(i) == 1)
		{
			if (i == (int)m_contrellerTeam.size() - 1)
			{
				m_controllerIndex.at(team2) = -1;
			}
			else
			{
				m_controllerIndex.at(team2) = i;
			}
			team2 += 2;
		}
	}

	m_stage->SetControllerIndex(m_controllerIndex);
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	m_charcterBg.clear();
	m_charcter.clear();
	m_bulletSample.clear();

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

	if (m_contrellerTextureBg == nullptr && m_peopleNumberSelect == nullptr && m_stageSelect == nullptr)
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

	if (m_stageIndex != m_stageSelect->GetSelectIdx().at(1))
	{
		m_stageIndex = m_stageSelect->GetSelectIdx().at(1);

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

	// �l���̐ݒ�
	if (m_peopleNumber != m_peopleNumberSelect->GetSelectIdx().at(1) * 2 + 2)
	{
		m_peopleNumber = m_peopleNumberSelect->GetSelectIdx().at(1) * 2 + 2;

		ResetCharcterSample();
	}

	CInput* input = CInput::GetKey();

	// ����{�^��
	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);

		// �I������
		if (m_stageSelect == nullptr)
		{
			m_peopleNumberSelect->SetIsDeleted();
			m_peopleNumberSelect->Uninit();
			delete m_peopleNumberSelect;
			m_peopleNumberSelect = nullptr;
		}
		else
		{
			assert(false);
		}

		// �������J�E���g����int�^���`�[�������쐬����B
		m_winNumber.resize(m_peopleNumber);
		for (int i = 0; i < (int)m_winNumber.size(); i++)
		{
			m_winNumber.at(i) = 0;
		}

		if (m_contrellerTextureBg == nullptr)
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
		}
		else
		{
			assert(false);
		}

		PeopleNumberSelectDelete();

		StageSelectInit();
	}
}

//-----------------------------------------------------------------------------
// �L�����N�^�[�I�𒆍X�V
//-----------------------------------------------------------------------------
void CGame::CharctorSelect()
{
	if (m_contrellerTextureBg == nullptr)
	{
		return;
	}

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	CInput* input = CInput::GetKey();
	std::vector<int> deviceIndexLeft = input->TriggerDevice(KEY_LEFT);
	std::vector<int> deviceIndexRigth = input->TriggerDevice(KEY_RIGHT);

	// �E�̓��͂��Ǘ�
	for (int i = 0; i < (int)deviceIndexLeft.size(); i++)
	{
		int index = deviceIndexLeft[i];

		if (index == -1)
		{
			index = m_contrellerTeam.size() - 1;
		}

		if (m_contrellerTeam.at(index) <= -1)
		{
			continue;
		}

		/* ���s���߂������͂���Ȃ��ꍇ�� */

		int chack = 0;
		for (int j = 0; j < (int)m_contrellerTeam.size(); j++)
		{
			// �`�[���ɂ��łɉ��l�������Ă��邩���ׂ�
			if (m_contrellerTeam.at(j) == -1)
			{
				chack++;
			}
		}

		if (chack >= m_peopleNumber * 0.5f && (m_contrellerTeam.at(index) - 1 == -1))
		{
			// �S�̂̔����ȏ�̐l���������ꍇ
			return;
		}

		/* ���v���C���[�̐l�����`�[���̋��e�ʈȉ��̏ꍇ�� */

		m_contrellerTeam.at(index)--;
	}

	// ���̓��͂��Ǘ�
	for (int i = 0; i < (int)deviceIndexRigth.size(); i++)
	{
		int index = deviceIndexRigth[i];

		if (index == -1)
		{
			index = m_contrellerTeam.size() - 1;
		}

		if (m_contrellerTeam.at(index) >= 1)
		{
			continue;
		}

		/* ���s���߂������͂���Ȃ��ꍇ�� */
		int chack = 0;
		for (int j = 0; j < (int)m_contrellerTeam.size(); j++)
		{
			// �`�[���ɂ��łɉ��l�������Ă��邩���ׂ�
			if (m_contrellerTeam.at(j) == 1)
			{
				chack++;
			}
		}

		if (chack >= m_peopleNumber * 0.5f && (m_contrellerTeam.at(index) + 1 == 1))
		{
			// �S�̂̔����ȏ�̐l���������ꍇ
			return;
		}

		m_contrellerTeam.at(index)++;
	}

	int team1 = 0;
	int team2 = m_peopleNumber * 0.5f;
	for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
	{
		switch (m_contrellerTeam.at(i))
		{
		case 0:
			m_contrellerTexture.at(i)->SetPos(m_contrellerPos.at(i));
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));
			break;
		case -1:
		{
			D3DXVECTOR3 pos = m_charcter.at(team1)->GetPos();
			pos.y += 60.0f;
			m_contrellerTexture.at(i)->SetPos(pos);
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));
			team1++;
		}
			break;
		case 1:
		{
			D3DXVECTOR3 pos = m_charcter.at(team2)->GetPos();
			pos.y += 60.0f;
			m_contrellerTexture.at(i)->SetPos(pos);
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(0));
			team2++;
		}
		break;
		default:
			break;
		}
	}

	// ����{�^��
	if (input->Trigger(KEY_DECISION))
	{
		// �R���g���[���[�̔ԍ���ݒ肷��B
		SetControllerIndex();

		// �w�i�̍폜
		m_contrellerTextureBg->SetIsDeleted(true);
		m_contrellerTextureBg = nullptr;

		// �R���g���[���[�e�N�X�`���̍폜
		for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
		{
			m_contrellerTexture.at(i)->SetIsDeleted(true);
		}
		m_contrellerTexture.clear();

		PeopleNumberSelectDelete();

		// �J�E���g�_�E���̏�����
		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// �ʒu��ݒ�
		m_countDownUI = CCountDownUI::Create(pos);	// �J�E���g�_�E���̊J�n
	}

	// �߂�{�^��
	if (input->Trigger(KEY_BACK))
	{
		if (m_stageSelect == nullptr)
		{
			// �w�i�̍폜
			m_contrellerTextureBg->SetIsDeleted(true);
			m_contrellerTextureBg = nullptr;

			// �R���g���[���[�e�N�X�`���̍폜
			for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
			{
				m_contrellerTexture.at(i)->SetIsDeleted(true);
			}
			m_contrellerTexture.clear();

			PeopleNumberSelectDelete();

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
					m_winNumber.at(i) = 0;
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

			if (index.at(0) == 1)
			{
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			if (index.at(0) == 0)
			{
				for (int i = 0; i < (int)m_winNumber.size(); i++)
				{
					m_winNumber.at(i) = 0;
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
	m_winNumber.at(m_winnerIndex)++;

	bool isRoundCountWon = false;
	for (int i = 0; i < (int)m_winNumber.size(); i++)
	{
		if (m_winNumber.at(i) >= m_needWinNumber)
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
	m_stage->SetControllerIndex(m_controllerIndex);

}
