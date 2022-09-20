//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "tutorial.h"
#include "stage.h"
#include "ui_countdown.h"
#include "ui_obtained_setnumber.h"
#include "ui_end_game.h"
#include "pause.h"
#include "menu.h"
#include "input.h"
#include <assert.h>
#include <functional>
#include "sound.h"

#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTutorial::CTutorial() :
	m_stageIndex(0),
	m_stage(nullptr),
	m_peopleNumberSelect(nullptr),
	m_charcterSelect(nullptr)
{
	m_stageInfo.clear();
	m_controllerIndex.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTutorial::~CTutorial()
{
	assert(m_charcterSelect == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CTutorial::Init()
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
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage02.json")["STAGE"];

	ResetStage();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTutorial::Uninit()
{
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
void CTutorial::Update()
{
	//// �X�e�[�W�I��
	//StageSelectUpdate();
	//if (m_stageSelect != nullptr)
	//{
	//	if (CInput::GetKey()->Trigger(KEY_BACK))
	//	{
	//		m_stageSelect->Uninit();
	//		delete m_stageSelect;
	//		m_stageSelect = nullptr;

	//		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
	//		//	��ʂ̑J��
	//		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
	//		return;
	//	}
	//}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CTutorial::Draw()
{
}

//-----------------------------------------------------------------------------
// �X�e�[�W�̃��Z�b�g
//-----------------------------------------------------------------------------
void CTutorial::ResetStage()
{
	if (m_stage != nullptr)
	{
		m_stage->AllDelete();	// �X�e�[�W��ō�����I�u�W�F�N�g��S�ĉ��
		m_stage->Uninit();	// �I��
		delete m_stage;
		m_stage = nullptr;
	}

	m_stage = new CStage;
	m_stage->Init(m_stageInfo[m_stageIndex]);	// ������
}
