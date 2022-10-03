//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "customize.h"
#include <assert.h>

#include "application.h"
#include "object2d.h"
#include "menu.h"
#include "menu_color_select.h"
#include "input.h"
#include "map.h"
#include "file.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCustomize::CCustomize() :
	m_bg(nullptr),
	m_stage(nullptr),
	m_manu(nullptr)
{

}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCustomize::~CCustomize()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CCustomize::Init()
{
	CApplication* application = CApplication::GetInstance();

	// �w�i�̐ݒ�
	{
		m_bg = CObject2D::Create(CObject::TYPE::NONE,1);
		m_bg->SetSize(D3DXVECTOR2((float)application->SCREEN_WIDTH, (float)application->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(application->CENTER_X, application->CENTER_Y, 0.0f);	// �ʒu�̎擾
		m_bg->SetTexture("BG");
		m_bg->SetPos(pos);
		m_bg->SetColor(application->GetColor(2));
	}

	m_manu = CColorSelectMenu::Create(application->GetColorSize());

	m_stage = new CMap;
	if (FAILED(m_stage->Init()))
	{
		return E_FAIL;
	}
	m_stage->SetMap(LoadJsonStage(L"data/FILE/STAGE/stage01.json")["STAGE"][0]["MAP"]);
	m_stage->Set();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CCustomize::Uninit()
{
	if (m_manu != nullptr)
	{
		m_manu->SetIsDeleted();
		m_manu->Uninit();
		delete m_manu;
		m_manu = nullptr;
	}

	// �}�b�v�̉��
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
void CCustomize::Update()
{
	auto SetColor = [this]()
	{
		CApplication::GetInstance()->SetThemeColor(m_manu->GetSelectIdx()[1]);
		m_bg->SetColor(CApplication::GetInstance()->GetColor(2));

		// �}�b�v�̉��
		m_stage->Delete();
		if (m_stage != nullptr)
		{
			m_stage->Uninit();
			delete m_stage;
			m_stage = nullptr;
		}
		m_stage = new CMap;
		if (FAILED(m_stage->Init()))
		{
			return;
		}
		m_stage->SetMap(LoadJsonStage(L"data/FILE/STAGE/stage01.json")["STAGE"][0]["MAP"]);
		m_stage->Set();

	};

	CInput* input = CInput::GetKey();

	if (m_manu != nullptr)
	{
		m_manu->Update();
	}

	if (input->Trigger(KEY_DECISION))
	{
		SetColor();
	}
	if (CInput::GetKey()->Trigger(KEY_BACK))
	{
		m_manu->SetIsDeleted();
		m_manu->Uninit();
		delete m_manu;
		m_manu = nullptr;

		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
		//	��ʂ̑J��
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
		return;
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CCustomize::Draw()
{
}
