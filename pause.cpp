#include "pause.h"
#include "object.h"
#include "menu.h"
#include "menu_item.h"
#include "application.h"
#include "input.h"
#include <assert.h>

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CPause::CPause() :
	m_bg(nullptr),
	m_menu(nullptr),
	m_status(BACK),
	m_isDeleted(false)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CPause::~CPause()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CPause::Init()
{
	CObject::SetStopUpdate(true);

	// �w�i�̐ݒ�
	{
		m_bg = CObject2D::Create(CObject::TYPE::NONE, 2);
		m_bg->SetSize(CApplication::GetInstance()->GetScreenSize());
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		m_bg->SetTexture("BG");
		m_bg->SetPos(pos);
		m_bg->SetColor(CApplication::GetInstance()->GetColor(0));
		m_bg->SetColorAlpha(0.25f);
	}

	// ���j���[�̐ݒ�
	{
		// �t���[���̐ݒ�
		CMenuFream* fream = new CMenuFream;
		{
			fream->Init();
			fream->SetColor(CApplication::GetInstance()->GetColor(1));
			fream->SetColorAlpha(0.75f);
		}

		// ���ڂ̐ݒ�
		std::vector<std::vector<CMenuItem*>> items;
		for (int i = 0; i < (int)CPause::Status::MAX; i++)
		{
			std::vector<CMenuItem*> X;

			CMenuItem* item = new CPauseMenuItem;
			item->Init();
			item->SetShouldStopAlsoUpdate();
			item->SetSize(D3DXVECTOR2(465.0f, 80.0f));			// �傫���̐ݒ�
			item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

			switch ((CPause::Status)i)
			{
			case CPause::Status::BACK:
				item->SetTexture("TEXT_EXIT");
				break;
			case CPause::Status::RESTART:
				item->SetTexture("TEXT_RESTART");
				break;
			case CPause::Status::END:
				item->SetTexture("TEXT_END");
				break;
			default:
				assert(false);
				break;
			}

			X.push_back(item);
			items.push_back(X);
		}

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
		D3DXVECTOR2 area((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->CENTER_Y * 0.8f);
		m_menu = CMenu::Create(pos, area, fream);
		m_menu->SetItems(items);
	}

	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CPause::Uninit()
{
	SetIsDeleted();
	CObject::SetStopUpdate(false);
	if (m_menu != nullptr)
	{
		m_menu->Uninit();
		delete m_menu;
		m_menu = nullptr;
	}
	if (m_bg != nullptr)
	{
		m_bg = nullptr;
	}
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CPause::Update()
{
	if (m_menu != nullptr)
	{
		m_menu->Update();

		CInput* input = CInput::GetKey();
		if (input->Trigger(KEY_DECISION))
		{
			m_status = (Status)m_menu->GetSelectIdx()[0];
			SetIsDeleted();
		}
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CPause::Draw()
{

}

//-----------------------------------------
// ���S��Ԃ̐ݒ�
//-----------------------------------------
void CPause::SetIsDeleted()
{
	m_isDeleted = true;
	if (m_bg != nullptr)
	{
		m_bg->SetIsDeleted(true);
	}
	if (m_menu != nullptr)
	{
		m_menu->SetIsDeleted();
	}
}
