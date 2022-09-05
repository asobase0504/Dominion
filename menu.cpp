//=========================================
// 
// メニュークラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "menu.h"
#include "application.h"

//-----------------------------------------
// 定義
//-----------------------------------------
const int CMenu::DECISION_AFTERGLOW_TIME = 30;

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CMenu::CMenu() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Area(D3DXVECTOR2(0.0f, 0.0f)),
	m_fream(nullptr),
	m_item({}),
	m_selectIdx({0,0}),
	m_fInterval(D3DXVECTOR2(0.0f, 0.0f)),
	m_AroundWhitespace(D3DXVECTOR2(0.0f, 0.0f)),
	m_isDexision(false),
	m_decisionAfterglowCount(0)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CMenu::~CMenu()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CMenu::Init()
{
	m_fInterval = { 5.0f,25.0f };
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CMenu::Uninit()
{
	m_item.clear();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CMenu::Update()
{
	if (!m_isDexision)
	{
		UpdateBeforeDecision();
	}
	else
	{
		UpdateAfterDecision();
	}
}

//-----------------------------------------
// 決定前の更新
//-----------------------------------------
void CMenu::UpdateBeforeDecision()
{
	for (int i = 0; i < m_item.size(); i++)
	{
		for (int j = 0; j < m_item[i].size(); j++)
		{
			if (m_selectIdx[0] == i && m_selectIdx[1] == j)
			{
				// 自身が選択中の場合
				m_item[i][j]->SelectUpdate();
			}
			else
			{
				// 自身が選択中ではない場合
				m_item[i][j]->NomalUpdate();
			}
		}
	}
}

//-----------------------------------------
// 決定後の更新
//-----------------------------------------
void CMenu::UpdateAfterDecision()
{
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CMenu::Draw()
{
}

//-----------------------------------------
// 選択
//-----------------------------------------
void CMenu::Select(SELECT_DIRECTION inDirection)
{
	switch (inDirection)
	{
	case CMenu::TOP:
		m_selectIdx[0]--;
		if (m_selectIdx[0] < 0)
		{
			m_selectIdx[0] = (int)m_item.size() - 1;
		}
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::DOWN:
		m_selectIdx[0]++;
		if (m_selectIdx[0] >= (int)m_item.size())
		{
			m_selectIdx[0] = 0;
		}
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::LEFT:
		m_selectIdx[1]--;
		if (m_selectIdx[1] < 0)
		{
			m_selectIdx[1] = (int)m_item[m_selectIdx[0]].size() - 1;
		}
		break;
	case CMenu::RIGHT:
		m_selectIdx[1]++;
		if (m_selectIdx[1] >= (int)m_item[m_selectIdx[0]].size())
		{
			m_selectIdx[1] = 0;
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------
// 決定
//-----------------------------------------
bool CMenu::Decision(bool inDecison)
{
	// 決定したか否か
	if (!m_isDexision)
	{
		m_isDexision = inDecison;
		return false;
	}

	/* ↓決定している場合↓ */

	m_decisionAfterglowCount++;

	bool check = m_decisionAfterglowCount >= DECISION_AFTERGLOW_TIME;
	if (check)
	{
		m_isDexision = false;
	}
	return check;
}

//-----------------------------------------
// 選択された番号の設定
//-----------------------------------------
void CMenu::SetSelectIdx(int Y, int X)
{
	m_selectIdx[0] = Y;
	m_selectIdx[1] = X;
}

//-----------------------------------------
// 選択された番号の設定
//-----------------------------------------
void CMenu::SetSelectIdx(std::vector<int> inIdx)
{
	m_selectIdx = inIdx;
}

//-----------------------------------------
// 作成
//-----------------------------------------
CMenu * CMenu::Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea, CMenuFream * inFream, std::vector<std::vector<CMenuItem*>> inItem)
{
	CMenu* menu = new CMenu;

	if (menu == nullptr)
	{
		return nullptr;
	}

	menu->Init();

	menu->m_pos = inPos;	// 位置の設定
	menu->m_fream = inFream;	// フレームの設定

	if (menu->m_fream != nullptr)
	{
		menu->m_fream->SetPos((D3DXVECTOR3)menu->m_pos);	// フレームの位置を設定
		menu->m_Area = inArea;	// 範囲の設定
		menu->m_fream->SetSize((D3DXVECTOR2)menu->m_Area);	// フレームの大きさを設定
	}

	menu->m_item = inItem;	// 項目の設定

	menu->SetItemPos();

	return menu;
}

//-----------------------------------------
// アイテムの位置を設定する
//-----------------------------------------
void CMenu::SetItemPos()
{
	for (int i = 0; i < m_item.size(); i++)
	{
		for (int j = 0; j < m_item.at(i).size(); j++)
		{
			CMenuItem* item = m_item[i][j];
			D3DXVECTOR2 pos = m_pos;
			pos.y -= ((m_item.size() - 1) * 0.5f) * (item->GetSize().y + m_fInterval.y);
			pos.x -= ((m_item.at(i).size() - 1) * 0.5f) * (item->GetSize().x + m_fInterval.x);

			pos.y += i * (item->GetSize().y + m_fInterval.y);
			pos.x += j * (item->GetSize().x + m_fInterval.x);

			item->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
		}
	}

}

//=========================================
// 
// フレームクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CMenuFream::CMenuFream(CObject::TYPE type) :
	CObject2D(type)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CMenuFream::~CMenuFream()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CMenuFream::Init()
{
	CObject2D::Init();
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CMenuFream::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CMenuFream::Update()
{
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CMenuFream::Draw()
{
	CObject2D::Draw();
}

//=========================================
// 
// 項目クラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CMenuItem::CMenuItem(CObject::TYPE type) :
	CObject2D(type)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CMenuItem::~CMenuItem()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CMenuItem::Init()
{
	CObject2D::Init();
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CMenuItem::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CMenuItem::Update()
{
}

//-----------------------------------------
// 選択状態のアップデート
//-----------------------------------------
void CMenuItem::SelectUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(3));
}

//-----------------------------------------
// 通常状態のアップデート
//-----------------------------------------
void CMenuItem::NomalUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CMenuItem::Draw()
{
	CObject2D::Draw();
}
