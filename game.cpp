//=============================================================================
// 
// アプリケーションクラス
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
#include "input.h"
#include <assert.h>
#include <functional>
#include "sound.h"

#include "application.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGame::CGame() :
	m_pause(nullptr),
	m_stageSelect(nullptr),
	m_stageIndex(0),
	m_stage(nullptr),
	m_peopleNumberSelect(nullptr),
	m_charcterSelect(nullptr),
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_endGameUI(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0)
{
	m_stageInfo.clear();
	m_controllerIndex.clear();
	m_winNumber.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(m_stageSelect == nullptr);
	assert(m_endGameUI == nullptr);
	assert(m_stage == nullptr);
	assert(m_peopleNumberSelect == nullptr);
	assert(m_charcterSelect == nullptr);
	assert(m_countDownUI == nullptr);
	assert(m_charcterSelect == nullptr);
	assert(m_obtainedSetNumberUI == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// 背景の設定
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 0);
		bg->SetSize(CApplication::GetInstance()->GetScreenSize());
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// 位置の取得
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}

	// ステージ情報の取得
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage01.json")["STAGE"];

	StageSelectInit();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ステージ選択中初期化
//-----------------------------------------------------------------------------
void CGame::StageSelectInit()
{
	// 仮ステージの設置
	ResetStage();

	// フレームの設定
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < (int)m_stageInfo.size(); i++)
	{

		CMenuItem* item = new CMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(80.0f, 80.0f));			// 大きさの設定
		item->SetColor(CApplication::GetInstance()->GetColor(0));			// 色の設定

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;
	m_stageSelect = CMenu::Create(CApplication::GetInstance()->GetScreenCenter(), area, fream, items);
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
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

	if (m_pause != nullptr)
	{
		m_pause->Uninit();
		delete m_pause;
		m_pause = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGame::Update()
{
	// ステージ選択中
	StageSelectUpdate();
	if (m_stageSelect != nullptr)
	{
		if (CInput::GetKey()->Trigger(KEY_BACK))
		{
			m_stageSelect->Uninit();
			delete m_stageSelect;
			m_stageSelect = nullptr;
			
			CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
			//	画面の遷移
			CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			return;
		}
	}

	// 人数選択中
	PeopleNumberSelectUpdate();

	// キャラクター選択中
	CharctorSelect();

	// バトル中
	BattleUpdate();
}

//-----------------------------------------------------------------------------
// ステージ選択中更新
//-----------------------------------------------------------------------------
void CGame::StageSelectUpdate()
{
	if (m_stageSelect == nullptr)
	{
		return;
	}

	m_stageSelect->Update();

	CInput* input = CInput::GetKey();
	if (input->Trigger(KEY_UP))
	{
		m_stageSelect->Select(CMenu::TOP);
	}
	if (input->Trigger(KEY_DOWN))
	{
		m_stageSelect->Select(CMenu::DOWN);
	}
	if (input->Trigger(KEY_LEFT))
	{
		m_stageSelect->Select(CMenu::LEFT);
	}
	if (input->Trigger(KEY_RIGHT))
	{
		m_stageSelect->Select(CMenu::RIGHT);
	}

	if (m_stageIndex != m_stageSelect->GetSelectIdx()[1])
	{
		m_stageIndex = m_stageSelect->GetSelectIdx()[1];
		// 仮ステージの設置
		ResetStage();
	}

	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
		m_stageSelect->SetIsDeleted();
		m_stageSelect->Uninit();
		delete m_stageSelect;
		m_stageSelect = nullptr;

		// 勝ちをカウントするint型をチーム数分作成する。
		m_winNumber.resize(2);
		for (int i = 0; i < (int)m_winNumber.size(); i++)
		{
			m_winNumber[i] = 0;
		}

		m_needWinNumber = 3;	// 勝利に必要なラウンド数の設定

		// コントローラーの番号をプレイヤー数分作成する
		m_controllerIndex.resize(2);
		m_controllerIndex[0] = -1;
		m_controllerIndex[1] = -2;

		// カウントダウンの初期化
		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
		m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始

	}
}

//-----------------------------------------------------------------------------
// 人数選択中更新
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectUpdate()
{
	if (m_peopleNumberSelect == nullptr)
	{
		return;
	}

	// 勝ちをカウントするint型をチーム数分作成する。
	m_winNumber.resize(2);
	for (int i = 0; i < (int)m_winNumber.size(); i++)
	{
		m_winNumber[i] = 0;
	}

	m_needWinNumber = 5;	// 勝利に必要なラウンド数の設定

}

//-----------------------------------------------------------------------------
// キャラクター選択中更新
//-----------------------------------------------------------------------------
void CGame::CharctorSelect()
{
	if (m_charcterSelect == nullptr)
	{
		return;
	}

	// コントローラーの番号をプレイヤー数分作成する
	m_controllerIndex.resize(2);
	m_controllerIndex[0] = -1;
	m_controllerIndex[1] = -2;

	// カウントダウンの初期化
	D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
	m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始
}

//-----------------------------------------------------------------------------
// バトル中更新
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
					m_winNumber[i] = 0;
				}
				// もう一度ゲームを行う
				ResetStage();
				// カウントダウンの初期化
				D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
				m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始
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

			// delete処理
			m_endGameUI->Uninit();
			delete m_endGameUI;
			m_endGameUI = nullptr;

			if (index[0] == 1)
			{
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			if (index[0] == 0)
			{
				for (int i = 0; i < (int)m_winNumber.size(); i++)
				{
					m_winNumber[i] = 0;
				}
				// もう一度ゲームを行う
				ResetStage();
				// カウントダウンの初期化
				D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
				m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始
			}
		}
		return;
	}

	// ラウンド終了UIの更新
	if (m_obtainedSetNumberUI != nullptr)
	{
		m_obtainedSetNumberUI->Update();
		if (m_obtainedSetNumberUI->GetIsDeleted())
		{
			// もう一度ゲームを行う
			ResetStage();
			// カウントダウンの初期化
			D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
			m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始

			// delete処理
			m_obtainedSetNumberUI->Uninit();
			delete m_obtainedSetNumberUI;
			m_obtainedSetNumberUI = nullptr;
		}
		return;
	}

	/* ↓ラウンド終了UIが存在しない場合↓ */

	// カウントダウンUIの更新
	if (m_countDownUI != nullptr)
	{
		m_countDownUI->Update();
		if (m_countDownUI->GetIsDeleted())
		{
			// delete処理
			m_countDownUI->Uninit();
			delete m_countDownUI;
			m_countDownUI = nullptr;
		}
	}

	/* ↓ラウンド開始のカウントダウンUIが存在しない場合↓ */

	if (m_stage == nullptr)
	{
		return;
	}

	/* ↓Stageが生成されてる場合↓ */

	// 戦闘中
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
	{ // 戦闘終了後
		BattleEnd();
	}
}

//-----------------------------------------------------------------------------
// バトル終了中更新
//-----------------------------------------------------------------------------
void CGame::BattleEnd()
{
	m_winnerIndex = m_stage->GetWinnerIndex();
	m_winNumber[m_winnerIndex]++;

	bool isRoundCountWon = false;
	for (int i = 0; i < (int)m_winNumber.size(); i++)
	{
		if (m_winNumber[i] >= m_needWinNumber)
		{
			isRoundCountWon = true;
		}
	}

	if (isRoundCountWon)
	{ // 指定数分どちらかのチームが勝った場合
		// エンドゲームUIの表示
		assert(m_endGameUI == nullptr);	// ここを通った段階でUIが存在していた場合警告
		m_endGameUI = CEndGameUI::Create(CApplication::GetInstance()->GetScreenCenter());
	}
	else
	{
		// ラウンド終了UIの表示
		assert(m_obtainedSetNumberUI == nullptr);	// ここを通った段階でUIが存在していた場合警告
		m_obtainedSetNumberUI = CObtainedSetNumberUI::Create(CApplication::GetInstance()->GetScreenCenter());
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}

//-----------------------------------------------------------------------------
// ステージのリセット
//-----------------------------------------------------------------------------
void CGame::ResetStage()
{
	if (m_stage != nullptr)
	{
		m_stage->AllDelete();	// ステージ状で作ったオブジェクトを全て解放
		m_stage->Uninit();	// 終了
		delete m_stage;
		m_stage = nullptr;
	}

	m_stage = new CStage;
	m_stage->Init(GetStageInfo());	// 初期化
}
