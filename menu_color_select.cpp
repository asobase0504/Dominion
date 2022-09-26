//=============================================================================
// 
// カラー選択メニュークラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "menu_color_select.h"
#include "application.h"
#include "sound.h"
#include "input.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenu::CColorSelectMenu()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenu::~CColorSelectMenu()
{
}

//-----------------------------------------------------------------------------
// 決定前の更新
//-----------------------------------------------------------------------------
void CColorSelectMenu::UpdateBeforeDecision()
{
	CMenu::UpdateBeforeDecision();
	// フレーム色の設定
	m_fream->SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------------------------------------------
// 決定後の更新
//-----------------------------------------------------------------------------
void CColorSelectMenu::UpdateAfterDecision()
{
}

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CColorSelectMenu* CColorSelectMenu::Create(int inColorCount)
{
	// フレームの設定
	CMenuFream* fream = new CColorSelectMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < inColorCount; i++)
	{

		CMenuItem* item = new CColorSelectMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(60.0f, 60.0f));			// 大きさの設定
		item->SetColor(CApplication::GetInstance()->GetColor(0));			// 色の設定

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;

	D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
	CColorSelectMenu* menu = (CColorSelectMenu*)CMenu::Create(pos, area, fream);
	menu->SetInterval({ 25.0f,25.0f });
	menu->SetItems(items);

	return menu;
}

//=============================================================================
// 
// チーム選択メニューフレームクラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenuFream::CColorSelectMenuFream(CObject::TYPE type) :
	CMenuFream(type)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenuFream::~CColorSelectMenuFream()
{
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CColorSelectMenuFream::Uninit()
{
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CColorSelectMenuFream::Update()
{
}

//=============================================================================
// 
// チーム選択メニューアイテムクラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenuItem::CColorSelectMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_SinTime(0)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CColorSelectMenuItem::~CColorSelectMenuItem()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CColorSelectMenuItem::Init()
{
	{
		m_EmphasisSelect = new CObject2D;
		m_EmphasisSelect->Init();
		m_EmphasisSelect->SetSize(D3DXVECTOR2(80.0f, 80.0f));
		m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
	}
	CMenuItem::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CColorSelectMenuItem::Uninit()
{
	if (m_EmphasisSelect != nullptr)
	{
		m_EmphasisSelect->SetIsDeleted(true);
		m_EmphasisSelect->Uninit();
		m_EmphasisSelect = nullptr;
	}
	CMenuItem::Uninit();
}

//-----------------------------------------------------------------------------
// 出現状態の更新
//-----------------------------------------------------------------------------
void CColorSelectMenuItem::PopUpdate()
{
	m_EmphasisSelect->SetPos(m_pos);
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// 選択状態の更新
//-----------------------------------------------------------------------------
void CColorSelectMenuItem::SelectUpdate()
{
	m_SinTime++;
	SetColor(CApplication::GetInstance()->GetColor(0));	// 色の設定
	D3DXVECTOR2 size(80.0f, 80.0f);
	size.x += 5.0f * sinf(0.095f * m_SinTime);
	size.y += 5.0f * sinf(0.095f * m_SinTime);
	m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));
	m_EmphasisSelect->SetSize(size);
	m_EmphasisSelect->SetColorAlpha(1.0f);
}

//-----------------------------------------------------------------------------
// 通常状態の更新
//-----------------------------------------------------------------------------
void CColorSelectMenuItem::NomalUpdate()
{
	m_SinTime = 0;
	SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
	m_EmphasisSelect->SetColorAlpha(0.0f);
}

//-----------------------------------------------------------------------------
// 終了状態の更新
//-----------------------------------------------------------------------------
void CColorSelectMenuItem::EndUpdate()
{
}
