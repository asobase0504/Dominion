//=============================================================================
//
// ラウンド取得数を表示するクラス
// Author YudaKaito
//
//=============================================================================
#include "ui_end_game.h"
#include "application.h"
#include "game.h"
#include "input.h"
#include "menu.h"

//-----------------------------------------------------------------------------
// 定義
//-----------------------------------------------------------------------------
const int CEndGameUI::PRIORITY_BELONG = 5;	// プライオリティ
const int CEndGameUI::NEW_SCORE_END_IN_TIME = 60;		// 新規得点の出現が終わる時間
const int CEndGameUI::NEXT_STAGE_TIME = 160;	// 次のステージへ移行する

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CEndGameUI * CEndGameUI::Create(const D3DXVECTOR2 & inPos)
{
	CEndGameUI* endgameUI = new CEndGameUI;

	endgameUI->Init();

	// フレームの初期化
	{
		CObject2D*& fream = endgameUI->m_fream;
		fream = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);
		if (FAILED(fream->Init()))
		{
			return nullptr;
		}
		D3DXVECTOR3 pos = (D3DXVECTOR3)inPos;
		pos.y *= 0.85f;
		fream->SetPos(pos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(0);
		color.a = 0.95f;
		fream->SetColor(color);
		fream->SetSize({ (float)CApplication::GetInstance()->SCREEN_WIDTH,90.0f });
		fream->SetShouldStopAlsoUpdate();
	}

	int teem = 2;
	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();

	// ラウンド数の取得
	{
		std::vector<std::vector<CObject2D*>>& setCountUI = endgameUI->m_setCountUI;	// 短い変数に変更

		setCountUI.resize(teem);	// チームの数分大きさを確保

		int needWinNumber = modeGame->GetNeedWinNumber();

		for (int i = 0; i < (int)setCountUI.size(); i++)
		{
			setCountUI[i].resize(needWinNumber);

			for (int j = 0; j < (int)setCountUI[i].size(); j++)
			{
				setCountUI[i][j] = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);

				// 初期化
				if (FAILED(setCountUI[i][j]->Init()))
				{
					return nullptr;
				}

				/* ↓２個目移行のオブジェクトだった場合↓ */

				D3DXVECTOR3 pos = (D3DXVECTOR3)inPos;
				pos.x = 0.0f;
				pos.y *= 0.85f;
				float center = (float)CApplication::GetInstance()->SCREEN_WIDTH / (float)teem;
				pos.x += center * i + center * 0.5f + ((j - setCountUI[i].size() * 0.5f) * 50.0f);
				setCountUI[i][j]->SetPos(pos);
				D3DXCOLOR color = CApplication::GetInstance()->GetColor(1);
				setCountUI[i][j]->SetColor(color);
				setCountUI[i][j]->SetSize({ 30.0f,30.0f });
				setCountUI[i][j]->SetShouldStopAlsoUpdate();
			}
		}
	}

	// 取得したラウンド数の取得
	{
		std::vector<std::vector<CObject2D*>>& tookSetUI = endgameUI->m_tookSetUI;	// 短い変数に変更

		tookSetUI.resize(teem);	// チームの数分大きさを確保

		for (int i = 0; i < (int)tookSetUI.size(); i++)
		{
			tookSetUI[i].resize(modeGame->GetWinNumber(i));

			int size = (int)tookSetUI[i].size();

			for (int j = 0; j < size; j++)
			{
				tookSetUI[i][j] = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);

				// 初期化
				if (FAILED(tookSetUI[i][j]->Init()))
				{
					return nullptr;
				}

				D3DXVECTOR3 pos = endgameUI->m_setCountUI[i][j]->GetPos();
				tookSetUI[i][j]->SetPos(pos);
				D3DXCOLOR color = CApplication::GetInstance()->GetColor(3);
				tookSetUI[i][j]->SetColor(color);
				if ((modeGame->GetWinner() == i) && ((int)tookSetUI[i].size() == j))
				{ // 勝者の場合
					tookSetUI[i][j]->SetSize({ 0.0f,0.0f });
				}
				else
				{
					tookSetUI[i][j]->SetSize({ 25.0f,25.0f });
				}

				tookSetUI[i][j]->SetShouldStopAlsoUpdate();
			}
		}
	}
	return endgameUI;
}

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CEndGameUI::CEndGameUI() :
	m_time(0),
	m_fream(nullptr),
	selectMenu(nullptr),
	m_pos(D3DXVECTOR2(0.0f, 0.0f)),
	m_isDeleted(false)
{
	m_tookSetUI.clear();
	m_setCountUI.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CEndGameUI::~CEndGameUI()
{
	assert(m_tookSetUI.empty());
	assert(m_setCountUI.empty());
	assert(m_fream == nullptr);
	assert(selectMenu == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CEndGameUI::Init()
{
	{ // メニュー
		CApplication* application = CApplication::GetInstance();
		// フレームの設定
		CMenuFream* fream = new CMenuFream;
		{
			fream->Init();
			fream->SetColor(application->GetColor(1));
			fream->SetColorAlpha(0.95f);
		}

		std::vector<std::vector<CMenuItem*>> items;
		for (int i = 0; i < 2; i++)
		{
			std::vector<CMenuItem*> X;
			CMenuItem* item = new CMenuItem;
			item->Init();
			switch (i)
			{
			case 0:
				item->SetTexture("TEXT_RETRY");
				break;
			case 1:
				item->SetTexture("TEXT_EXIT");
				break;
			default:
				assert(false);
				break;
			}
			item->SetSize(D3DXVECTOR2(465.0f, 80.0f));			// 大きさの設定
			item->SetColor(application->GetColor(0));			// 色の設定

			X.push_back(item);
			items.push_back(X);
		}

		D3DXVECTOR2 area = application->GetScreenSize();
		area.y *= 0.27f;
		D3DXVECTOR2 pos = application->GetScreenCenter();
		pos.y *= 1.25f;

		selectMenu = CMenu::Create(pos, area, fream, items);
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CEndGameUI::Uninit()
{
	m_tookSetUI.clear();
	m_setCountUI.clear();

	if (selectMenu != nullptr)
	{
		selectMenu->Uninit();
		delete selectMenu;
		selectMenu = nullptr;
	}

	if (m_fream != nullptr)
	{
		m_fream = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CEndGameUI::Update()
{
	m_time++;
	if (m_time <= NEW_SCORE_END_IN_TIME)
	{
		CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
		CObject2D* point = m_tookSetUI[modeGame->GetWinner()][m_tookSetUI[modeGame->GetWinner()].size() - 1];
		float transitionSize = 25.0f / NEW_SCORE_END_IN_TIME;

		point->SetSize(D3DXVECTOR2(transitionSize * m_time, transitionSize * m_time));
	}

	CInput* input = CInput::GetKey();

	if (selectMenu != nullptr)
	{
		selectMenu->Update();
		if (input->Trigger(KEY_UP))
		{
			selectMenu->Select(CMenu::TOP);
		}
		if (input->Trigger(KEY_DOWN))
		{
			selectMenu->Select(CMenu::DOWN);
		}
		if (input->Trigger(KEY_LEFT))
		{
			selectMenu->Select(CMenu::LEFT);
		}
		if (input->Trigger(KEY_RIGHT))
		{
			selectMenu->Select(CMenu::RIGHT);
		}
	}

	if (input->Trigger(KEY_DECISION))
	{
		SetIsDeleted();
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CEndGameUI::Draw()
{
}

//-----------------------------------------------------------------------------
// 解放予定の設定
//-----------------------------------------------------------------------------
void CEndGameUI::SetIsDeleted()
{
	m_isDeleted = true;
	for (int i = 0; i < (int)m_setCountUI.size(); i++)
	{
		for (int j = 0; j < (int)m_setCountUI[i].size(); j++)
		{
			m_setCountUI[i][j]->SetIsDeleted(true);
		}
	}

	for (int i = 0; i < (int)m_tookSetUI.size(); i++)
	{
		for (int j = 0; j < (int)m_tookSetUI[i].size(); j++)
		{
			m_tookSetUI[i][j]->SetIsDeleted(true);
		}
	} 
	m_fream->SetIsDeleted(true);
	selectMenu->SetIsDeleted();
}
