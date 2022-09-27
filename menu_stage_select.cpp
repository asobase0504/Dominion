//=============================================================================
// 
// ステージ選択メニュークラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "menu_stage_select.h"
#include "application.h"
#include "sound.h"
#include "input.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CStageSelectMenu::CStageSelectMenu()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CStageSelectMenu::~CStageSelectMenu()
{
}

//-----------------------------------------------------------------------------
// 決定前の更新
//-----------------------------------------------------------------------------
void CStageSelectMenu::UpdateBeforeDecision()
{
	CMenu::UpdateBeforeDecision();
}

//-----------------------------------------------------------------------------
// 決定後の更新
//-----------------------------------------------------------------------------
void CStageSelectMenu::UpdateAfterDecision()
{
}

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CStageSelectMenu* CStageSelectMenu::Create(int inStageCount)
{
	// フレームの設定
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < inStageCount; i++)
	{

		CMenuItem* item = new CStageSelectMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(60.0f, 60.0f));			// 大きさの設定
		item->SetColor(CApplication::GetInstance()->GetColor(0));			// 色の設定

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;

	D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
	CStageSelectMenu* menu = (CStageSelectMenu*)CMenu::Create(pos, area, fream);
	menu->SetInterval({25.0f,25.0f});
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
CStageSelectMenuFream::CStageSelectMenuFream(CObject::TYPE type) :
	CMenuFream(type)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CStageSelectMenuFream::~CStageSelectMenuFream()
{
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CStageSelectMenuFream::Uninit()
{
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CStageSelectMenuFream::Update()
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
CStageSelectMenuItem::CStageSelectMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_SinTime(0)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CStageSelectMenuItem::~CStageSelectMenuItem()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CStageSelectMenuItem::Init()
{
	{
		m_EmphasisSelect = new CObject2D;
		m_EmphasisSelect->Init();
		m_EmphasisSelect->SetSize(D3DXVECTOR2(0.0f, 0.0f));
		m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
	}
	CMenuItem::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::Uninit()
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
void CStageSelectMenuItem::PopUpdate()
{
	m_EmphasisSelect->SetPos(m_pos);
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// 選択状態の更新
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::SelectUpdate()
{
	m_SinTime++;
	SetColor(CApplication::GetInstance()->GetColor(0));	// 色の設定
	D3DXVECTOR2 size(80.0f, 80.0f);
	size.x += 5.0f * sinf(0.095f * m_SinTime);
	size.y += 5.0f * sinf(0.095f * m_SinTime);
	m_EmphasisSelect->SetSize(size);
	m_EmphasisSelect->SetColorAlpha(1.0f);
}

//-----------------------------------------------------------------------------
// 通常状態の更新
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::NomalUpdate()
{
	m_SinTime = 0;
	SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
	m_EmphasisSelect->SetColorAlpha(0.0f);
}

//-----------------------------------------------------------------------------
// 終了状態の更新
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::EndUpdate()
{
}
