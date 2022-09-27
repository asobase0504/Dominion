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
#include "menu_stage_select.h"
#include "menu_number_Select.h"
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
	m_countDownUI(nullptr),
	m_obtainedSetNumberUI(nullptr),
	m_endGameUI(nullptr),
	m_contrellerTextureBg(nullptr),
	m_needWinNumber(0),
	m_winnerIndex(0),
	m_peopleNumber(0)
{
	m_charcterBg.clear();
	m_charcter.clear();
	m_bulletSample.clear();
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
	assert(m_countDownUI == nullptr);
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
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage01.json").at("STAGE");

	StageSelectInit();

	m_needWinNumber = 1;
	m_peopleNumber = 2;
	return S_OK;
}

//-----------------------------------------------------------------------------
// ステージ選択中初期化
//-----------------------------------------------------------------------------
void CGame::StageSelectInit()
{
	// アップデートを通すラグをリセット
	m_updateLagTime = 0;

	// 仮ステージの設置
	ResetStage();

	// ステージセレクトメニューの作成
	m_stageSelect = CStageSelectMenu::Create(m_stageInfo.size());
}

//-----------------------------------------------------------------------------
// 人数選択
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectInit()
{
	m_updateLagTime = 0;

	// フレームの設定
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < 2; i++)
	{
		CNumberSelectMenuItem* item = new CNumberSelectMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(80.0f, 80.0f));					// 大きさの設定
		item->SetColor(CApplication::GetInstance()->GetColor(0));	// 色の設定
		item->SetAbovePasteTexture(i == 0 ? "2PL" : "4PL");	// アイテムの上に貼るオブジェクトのテクスチャ設定
		X.push_back(item);
	}
	items.push_back(X);

	{
		D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
		area.y *= 0.25f;
		D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
		pos.y *= 1.5f;
		m_peopleNumberSelect = CMenu::Create(pos, area, fream);
		m_peopleNumberSelect->SetInterval({ 25.0f,25.0f });
		m_peopleNumberSelect->SetItems(items);
	}

	m_charcterBg.resize(2);
	for (int i = 0; i < 2; i++)
	{
		m_charcterBg.at(i) = new CObject2D(CObject::TYPE::NONE,2);
		m_charcterBg.at(i)->Init();

		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.x *= (0.5f + 1.0f * i);
		pos.y *= 0.75f;
		m_charcterBg.at(i)->SetPos(pos);	// 色の設定
		m_charcterBg.at(i)->SetColor(CApplication::GetInstance()->GetColor(i == 0 ? 0 : 1));	// 色の設定
		D3DXVECTOR2 size(CApplication::GetInstance()->GetScreenSize());
		size.x *= 0.5f;
		size.y *= 0.25f;
		m_charcterBg.at(i)->SetSize(size);	// 色の設定
		m_charcterBg.at(i)->SetColorAlpha(0.95f);	// 色の設定
	}

	ResetCharcterSample();
}

//-----------------------------------------------------------------------------
// 人数選択の削除
//-----------------------------------------------------------------------------
void CGame::PeopleNumberSelectDelete()
{
	for (int i = 0; i < (int)m_charcterBg.size(); i++)
	{
		m_charcterBg.at(i)->SetIsDeleted(true);
	}
	m_charcterBg.clear();

	for (int i = 0; i < (int)m_charcter.size(); i++)
	{
		m_charcter.at(i)->SetIsDeleted(true);
	}
	m_charcter.clear();

	for (int i = 0; i < (int)m_bulletSample.size(); i++)
	{
		m_bulletSample.at(i)->SetIsDeleted(true);
		m_bulletSample.at(i)->Uninit();
	}
	m_bulletSample.clear();
}

//-----------------------------------------------------------------------------
// キャラクターsample
//-----------------------------------------------------------------------------
void CGame::ResetCharcterSample()
{
	for (int i = 0; i < (int)m_charcter.size(); i++)
	{
		m_charcter.at(i)->SetIsDeleted(true);
	}
	m_charcter.clear();

	for (int i = 0; i < (int)m_bulletSample.size(); i++)
	{
		m_bulletSample.at(i)->SetIsDeleted(true);
	}
	m_bulletSample.clear();

	m_charcter.resize(m_peopleNumber);
	m_bulletSample.resize(m_peopleNumber);
	for (int i = 0; i < m_peopleNumber; i++)
	{
		bool team = (m_peopleNumber * 0.5f) <= i;

		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X / m_peopleNumber, CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.y *= 0.75f;
		pos.x += pos.x * (i * 2);
		D3DXVECTOR2 size(60.0f, 60.0f);

		m_charcter.at(i) = new CObject2D;
		m_charcter.at(i)->Init();		// 初期化
		m_charcter.at(i)->SetPos(pos);	// 位置の設定
		m_charcter.at(i)->SetColor(CApplication::GetInstance()->GetColor(team ? 0 : 1));	// 色の設定
		m_charcter.at(i)->SetSize(size);	// 大きさの設定
		m_charcter.at(i)->SetTexture("PLAYER");	// テクスチャの設定

		m_bulletSample.at(i) = new CObject2D;
		m_bulletSample.at(i)->Init();		// 初期化
		m_bulletSample.at(i)->SetPos(pos);	// 位置の設定
		m_bulletSample.at(i)->SetColor(CApplication::GetInstance()->GetColor(team ? 1 : 0));	// 色の設定
		m_bulletSample.at(i)->SetSize(size);	// 大きさの設定
		m_bulletSample.at(i)->SetTexture("BULLET_SAMPLE");	// テクスチャの設定
	}
}

//-----------------------------------------------------------------------------
// キャラクター選択
//-----------------------------------------------------------------------------
void CGame::CharcterSelectInit()
{
	m_updateLagTime = 0;

	// 背景の設定
	if (m_contrellerTextureBg == nullptr)
	{
		m_contrellerTextureBg = new CObject2D;
		m_contrellerTextureBg->Init();
		m_contrellerTextureBg->SetColor(CApplication::GetInstance()->GetColor(0));
		m_contrellerTextureBg->SetColorAlpha(0.55f);
		D3DXVECTOR2 size = CApplication::GetInstance()->GetScreenSize();
		size.y *= 0.25f;
		m_contrellerTextureBg->SetSize(size);
		D3DXVECTOR3 pos(CApplication::GetInstance()->GetScreenCenter());
		pos.y *= 1.5f;
		pos.z = 0.0f;
		m_contrellerTextureBg->SetPos(pos);
	}

	// コントローラー誘導のテクスチャ
	m_contrellerTexture.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	m_contrellerTeam.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	m_contrellerPos.resize(CInput::GetKey()->GetAcceptJoyPadCount() + 1);
	for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
	{
		m_contrellerTeam.at(i) = 0;
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X / m_contrellerTexture.size(), CApplication::GetInstance()->CENTER_Y, 0.0f);
		pos.y *= 1.5f;
		pos.x += pos.x * (i * 2);
		D3DXVECTOR2 size(70.0f, 70.0f);

		m_contrellerTexture.at(i) = new CObject2D;
		m_contrellerTexture.at(i)->Init();		// 初期化
		m_contrellerPos.at(i) = pos;
		m_contrellerTexture.at(i)->SetPos(pos);	// 位置の設定
		m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
		m_contrellerTexture.at(i)->SetSize(size);	// 大きさの設定

		if (CInput::GetKey()->GetAcceptJoyPadCount() != i)
		{
			m_contrellerTexture.at(i)->SetTexture("CONTLLER");	// テクスチャの設定
		}
		else
		{
			m_contrellerTexture.at(i)->SetTexture("KEYBOAD");	// テクスチャの設定
		}
	}

	// コントローラーの番号をプレイヤー数分作成する
	m_controllerIndex.resize(m_peopleNumber);

	for (int i = 0; i < (int)m_controllerIndex.size(); i++)
	{
		m_controllerIndex.at(i) = -2;
	}
}

//-----------------------------------------------------------------------------
// コントローラーの番号を設定
//-----------------------------------------------------------------------------
void CGame::SetControllerIndex()
{
	int team1 = 0;
	int team2 = 1;
	for (int i = 0; i < (int)m_contrellerTeam.size(); i++)
	{
		if (m_contrellerTeam.at(i) == 0)
		{
			continue;
		}
		else if (m_contrellerTeam.at(i) == -1)
		{
			if (i == (int)m_contrellerTeam.size() - 1)
			{
				m_controllerIndex.at(team1) = -1;
			}
			else
			{
				m_controllerIndex.at(team1) = i;
			}
			team1 += 2;
		}
		else if (m_contrellerTeam.at(i) == 1)
		{
			if (i == (int)m_contrellerTeam.size() - 1)
			{
				m_controllerIndex.at(team2) = -1;
			}
			else
			{
				m_controllerIndex.at(team2) = i;
			}
			team2 += 2;
		}
	}

	m_stage->SetControllerIndex(m_controllerIndex);
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	m_charcterBg.clear();
	m_charcter.clear();
	m_bulletSample.clear();

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

	if (m_peopleNumberSelect != nullptr)
	{
		m_peopleNumberSelect->Uninit();
		delete m_peopleNumberSelect;
		m_peopleNumberSelect = nullptr;
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
			// 画面の遷移
			CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			return;
		}
	}

	// 人数選択中
	PeopleNumberSelectUpdate();

	// キャラクター選択中
	CharctorSelect();

	if (m_contrellerTextureBg == nullptr && m_peopleNumberSelect == nullptr && m_stageSelect == nullptr)
	{
		// バトル中
		BattleUpdate();
	}
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

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	m_stageSelect->Update();

	if (m_stageIndex != m_stageSelect->GetSelectIdx().at(1))
	{
		m_stageIndex = m_stageSelect->GetSelectIdx().at(1);

		ResetStage();	// 仮ステージの設置
	}

	CInput* input = CInput::GetKey();
	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
		m_stageSelect->SetIsDeleted();
		m_stageSelect->Uninit();
		delete m_stageSelect;
		m_stageSelect = nullptr;

		if (m_peopleNumberSelect == nullptr)
		{
			PeopleNumberSelectInit();
		}
		else
		{
			assert(false);
		}
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

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	m_peopleNumberSelect->Update();

	// 人数の設定
	if (m_peopleNumber != m_peopleNumberSelect->GetSelectIdx().at(1) * 2 + 2)
	{
		m_peopleNumber = m_peopleNumberSelect->GetSelectIdx().at(1) * 2 + 2;

		ResetCharcterSample();
	}

	CInput* input = CInput::GetKey();

	// 決定ボタン
	if (input->Trigger(KEY_DECISION))
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);

		// 終了処理
		if (m_stageSelect == nullptr)
		{
			m_peopleNumberSelect->SetIsDeleted();
			m_peopleNumberSelect->Uninit();
			delete m_peopleNumberSelect;
			m_peopleNumberSelect = nullptr;
		}
		else
		{
			assert(false);
		}

		// 勝ちをカウントするint型をチーム数分作成する。
		m_winNumber.resize(m_peopleNumber);
		for (int i = 0; i < (int)m_winNumber.size(); i++)
		{
			m_winNumber.at(i) = 0;
		}

		if (m_contrellerTextureBg == nullptr)
		{
			CharcterSelectInit();
			return;
		}
		else
		{
			assert(false);
		}
	}

	// 戻るボタン
	if (input->Trigger(KEY_BACK))
	{
		if (m_stageSelect == nullptr)
		{
			m_peopleNumberSelect->SetIsDeleted();
			m_peopleNumberSelect->Uninit();
			delete m_peopleNumberSelect;
			m_peopleNumberSelect = nullptr;
		}
		else
		{
			assert(false);
		}

		PeopleNumberSelectDelete();

		StageSelectInit();
	}
}

//-----------------------------------------------------------------------------
// キャラクター選択中更新
//-----------------------------------------------------------------------------
void CGame::CharctorSelect()
{
	if (m_contrellerTextureBg == nullptr)
	{
		return;
	}

	if (m_updateLagTime <= 0)
	{
		m_updateLagTime++;
		return;
	}

	CInput* input = CInput::GetKey();
	std::vector<int> deviceIndexLeft = input->TriggerDevice(KEY_LEFT);
	std::vector<int> deviceIndexRigth = input->TriggerDevice(KEY_RIGHT);

	// 右の入力を管理
	for (int i = 0; i < (int)deviceIndexLeft.size(); i++)
	{
		int index = deviceIndexLeft[i];

		if (index == -1)
		{
			index = m_contrellerTeam.size() - 1;
		}

		if (m_contrellerTeam.at(index) <= -1)
		{
			continue;
		}

		/* ↓行き過ぎた入力じゃない場合↓ */

		int chack = 0;
		for (int j = 0; j < (int)m_contrellerTeam.size(); j++)
		{
			// チームにすでに何人所属しているか調べる
			if (m_contrellerTeam.at(j) == -1)
			{
				chack++;
			}
		}

		if (chack >= m_peopleNumber * 0.5f && (m_contrellerTeam.at(index) - 1 == -1))
		{
			// 全体の半分以上の人数が居た場合
			return;
		}

		/* ↓プレイヤーの人数がチームの許容量以下の場合↓ */

		m_contrellerTeam.at(index)--;
	}

	// 左の入力を管理
	for (int i = 0; i < (int)deviceIndexRigth.size(); i++)
	{
		int index = deviceIndexRigth[i];

		if (index == -1)
		{
			index = m_contrellerTeam.size() - 1;
		}

		if (m_contrellerTeam.at(index) >= 1)
		{
			continue;
		}

		/* ↓行き過ぎた入力じゃない場合↓ */
		int chack = 0;
		for (int j = 0; j < (int)m_contrellerTeam.size(); j++)
		{
			// チームにすでに何人所属しているか調べる
			if (m_contrellerTeam.at(j) == 1)
			{
				chack++;
			}
		}

		if (chack >= m_peopleNumber * 0.5f && (m_contrellerTeam.at(index) + 1 == 1))
		{
			// 全体の半分以上の人数が居た場合
			return;
		}

		m_contrellerTeam.at(index)++;
	}

	int team1 = 0;
	int team2 = m_peopleNumber * 0.5f;
	for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
	{
		switch (m_contrellerTeam.at(i))
		{
		case 0:
			m_contrellerTexture.at(i)->SetPos(m_contrellerPos.at(i));
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));
			break;
		case -1:
		{
			D3DXVECTOR3 pos = m_charcter.at(team1)->GetPos();
			pos.y += 60.0f;
			m_contrellerTexture.at(i)->SetPos(pos);
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(1));
			team1++;
		}
			break;
		case 1:
		{
			D3DXVECTOR3 pos = m_charcter.at(team2)->GetPos();
			pos.y += 60.0f;
			m_contrellerTexture.at(i)->SetPos(pos);
			m_contrellerTexture.at(i)->SetColor(CApplication::GetInstance()->GetColor(0));
			team2++;
		}
		break;
		default:
			break;
		}
	}

	// 決定ボタン
	if (input->Trigger(KEY_DECISION))
	{
		// コントローラーの番号を設定する。
		SetControllerIndex();

		// 背景の削除
		m_contrellerTextureBg->SetIsDeleted(true);
		m_contrellerTextureBg = nullptr;

		// コントローラーテクスチャの削除
		for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
		{
			m_contrellerTexture.at(i)->SetIsDeleted(true);
		}
		m_contrellerTexture.clear();

		PeopleNumberSelectDelete();

		// カウントダウンの初期化
		D3DXVECTOR2 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y);	// 位置を設定
		m_countDownUI = CCountDownUI::Create(pos);	// カウントダウンの開始
	}

	// 戻るボタン
	if (input->Trigger(KEY_BACK))
	{
		if (m_stageSelect == nullptr)
		{
			// 背景の削除
			m_contrellerTextureBg->SetIsDeleted(true);
			m_contrellerTextureBg = nullptr;

			// コントローラーテクスチャの削除
			for (int i = 0; i < (int)m_contrellerTexture.size(); i++)
			{
				m_contrellerTexture.at(i)->SetIsDeleted(true);
			}
			m_contrellerTexture.clear();

			PeopleNumberSelectDelete();

			PeopleNumberSelectInit();
			return;
		}
		else
		{
			assert(false);
		}
	}
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
					m_winNumber.at(i) = 0;
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

			if (index.at(0) == 1)
			{
				CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			}
			if (index.at(0) == 0)
			{
				for (int i = 0; i < (int)m_winNumber.size(); i++)
				{
					m_winNumber.at(i) = 0;
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
		return;
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
	m_winNumber.at(m_winnerIndex)++;

	bool isRoundCountWon = false;
	for (int i = 0; i < (int)m_winNumber.size(); i++)
	{
		if (m_winNumber.at(i) >= m_needWinNumber)
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
	m_stage->SetControllerIndex(m_controllerIndex);

}
