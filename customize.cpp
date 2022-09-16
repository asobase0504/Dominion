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
#include "input.h"
#include "map.h"
#include "file.h"

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
	// �w�i�̐ݒ�
	{
		m_bg = CObject2D::Create(CObject::TYPE::NONE,1);
		m_bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		m_bg->SetTexture("BG");
		m_bg->SetPos(pos);
		m_bg->SetColor(CApplication::GetInstance()->GetColor(2));
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
		std::vector<CMenuItem*> X;
		for (int i = 0; i < CApplication::GetInstance()->GetColorSize(); i++)
		{

			CMenuItem* item = new CMenuItem;
			item->Init();
			item->SetSize(D3DXVECTOR2(80.0f, 80.0f));			// �傫���̐ݒ�
			item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

			X.push_back(item);
		}
		items.push_back(X);

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
		D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
		area.y *= 0.25f;
		m_manu = CMenu::Create(pos, area, fream, items);
	}

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

		m_manu->GetFream()->SetColor(CApplication::GetInstance()->GetColor(1));

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
		if (input->Trigger(DIK_W))
		{
			m_manu->Select(CMenu::TOP);
			SetColor();
		}
		if (input->Trigger(DIK_S))
		{
			m_manu->Select(CMenu::DOWN);
			SetColor();
		}
		if (input->Trigger(DIK_A))
		{
			m_manu->Select(CMenu::LEFT);
			SetColor();
		}
		if (input->Trigger(DIK_D))
		{
			m_manu->Select(CMenu::RIGHT);
			SetColor();
		}
	}
	if (CInput::GetKey()->Trigger(KEY_BACK) || input->Trigger(DIK_RETURN))
	{
		m_manu->Uninit();
		delete m_manu;
		m_manu = nullptr;

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