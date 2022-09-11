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
#include <assert.h>
#include <functional>

#include "application.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGame::CGame() :
	m_stage(nullptr),
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0)
{
	m_winNumber.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(m_countDownUI == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// 勝ちをカウントするint型をチーム数分作成する。
	m_winNumber.resize(2);
	for (int i = 0; i < m_winNumber.size(); i++)
	{
		m_winNumber[i] = 0;
	}

	m_needWinNumber = 5;
	// 背景の設定
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 0);
		bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// 位置の取得
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}

	m_stage = new CStage;

	m_stage->Init();

	m_countDownUI = CCountDownUI::Create(D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y));
	return S_OK;
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

	if (m_stage != nullptr)
	{
		m_stage->Uninit();
		delete m_stage;
		m_stage = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGame::Update()
{
	// ラウンド終了UIの更新
	if (m_obtainedSetNumberUI != nullptr)
	{
		m_obtainedSetNumberUI->Update();
		if (m_obtainedSetNumberUI->GetIsDeleted())
		{
			// もう一度ゲームを行う
			ResetStage();

			// delete処理
			m_obtainedSetNumberUI->Uninit();
			delete m_obtainedSetNumberUI;
			m_obtainedSetNumberUI = nullptr;
		}
		return;
	}

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

	if (m_stage != nullptr)
	{
		m_stage->Update();
		if (m_stage->GetEndSet())
		{
			m_winnerIndex = m_stage->GetWinnerIndex();
			m_winNumber[m_winnerIndex]++;

			auto isSetCheack = [this](int idx)
			{
				return m_winNumber[idx] >= m_needWinNumber;
			};

			bool isRoundCountWon = false;
			for (int i = 0; i < m_winNumber.size(); i++)
			{
				if (isSetCheack(i))
				{
					isRoundCountWon = true;
				}
			}

			if (isRoundCountWon)
			{ // 指定数分どちらかのチームが勝った場合
				//	画面の遷移
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			else
			{
				// もう一度やる前にUIで催促
				if (m_obtainedSetNumberUI != nullptr)
				{
					// ここを通った段階でUIが消えてなかった場合警告
					assert(false);
				}

				D3DXVECTOR2 pos = D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
				m_obtainedSetNumberUI = CObtainedSetNumberUI::Create(pos);
			}
		}
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
	m_stage->AllDelete();	// ステージ状で作ったオブジェクトを全て解放
	m_stage->Uninit();	// 終了
	delete m_stage;
	m_stage = nullptr;

	m_stage = new CStage;
	m_stage->Init();	// 初期化
	D3DXVECTOR2 pos = D3DXVECTOR2(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);
	m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始
}
