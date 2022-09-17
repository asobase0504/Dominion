#include "pause.h"
#include "object.h"
#include "menu.h"
#include "menu_item.h"
#include "application.h"
#include "input.h"
#include <assert.h>

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CPause::CPause() :
	m_bg(nullptr),
	m_menu(nullptr),
	m_status(BACK),
	m_isDeleted(false)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CPause::~CPause()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CPause::Init()
{
	CObject::SetStopUpdate(true);

	// メニューの設定
	{
		// フレームの設定
		CMenuFream* fream = new CMenuFream;
		{
			fream->Init();
			fream->SetColor(CApplication::GetInstance()->GetColor(1));
		}

		// 項目の設定
		std::vector<std::vector<CMenuItem*>> items;
		for (int i = 0; i < (int)CPause::Status::MAX; i++)
		{
			std::vector<CMenuItem*> X;

			CMenuItem* item = new CPauseMenuItem;
			item->Init();
			item->SetShouldStopAlsoUpdate();
			item->SetSize(D3DXVECTOR2(465.0f, 80.0f));			// 大きさの設定
			item->SetColor(CApplication::GetInstance()->GetColor(0));			// 色の設定

			switch ((CPause::Status)i)
			{
			case CPause::Status::BACK:
				item->SetTexture("TEXT_START");
				break;
			case CPause::Status::RESTART:
				item->SetTexture("TEXT_TUTORIAL");
				break;
			case CPause::Status::END:
				item->SetTexture("TEXT_CUSTOM");
				break;
			default:
				assert(false);
				break;
			}

			X.push_back(item);
			items.push_back(X);
		}

		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X * 1.55f, CApplication::GetInstance()->CENTER_Y * 1.2f);
		D3DXVECTOR2 area(500.0f, 550.0f);
		m_menu = CMenu::Create(pos, area, fream, items);
	}

	return S_OK;
}

//-----------------------------------------
// 終了
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
// 更新
//-----------------------------------------
void CPause::Update()
{
	m_menu->Update();

	CInput* input = CInput::GetKey();

	if (m_menu != nullptr)
	{
		m_menu->Update();
		if (input->Trigger(KEY_UP))
		{
			m_menu->Select(CMenu::TOP);
		}
		if (input->Trigger(KEY_DOWN))
		{
			m_menu->Select(CMenu::DOWN);
		}
		if (input->Trigger(KEY_LEFT))
		{
			m_menu->Select(CMenu::LEFT);
		}
		if (input->Trigger(KEY_RIGHT))
		{
			m_menu->Select(CMenu::RIGHT);
		}
		if (input->Trigger(KEY_DECISION))
		{
			m_status = (Status)m_menu->GetSelectIdx()[0];
			SetIsDeleted();
		}
	}
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CPause::Draw()
{

}

//-----------------------------------------
// 死亡状態の設定
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
