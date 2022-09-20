//=============================================================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "tutorial.h"
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
CTutorial::CTutorial() :
	m_stageIndex(0),
	m_stage(nullptr),
	m_peopleNumberSelect(nullptr),
	m_charcterSelect(nullptr)
{
	m_stageInfo.clear();
	m_controllerIndex.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CTutorial::~CTutorial()
{
	assert(m_charcterSelect == nullptr);
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CTutorial::Init()
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
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage02.json")["STAGE"];

	ResetStage();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CTutorial::Uninit()
{
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
void CTutorial::Update()
{
	//// ステージ選択中
	//StageSelectUpdate();
	//if (m_stageSelect != nullptr)
	//{
	//	if (CInput::GetKey()->Trigger(KEY_BACK))
	//	{
	//		m_stageSelect->Uninit();
	//		delete m_stageSelect;
	//		m_stageSelect = nullptr;

	//		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION);
	//		//	画面の遷移
	//		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
	//		return;
	//	}
	//}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CTutorial::Draw()
{
}

//-----------------------------------------------------------------------------
// ステージのリセット
//-----------------------------------------------------------------------------
void CTutorial::ResetStage()
{
	if (m_stage != nullptr)
	{
		m_stage->AllDelete();	// ステージ状で作ったオブジェクトを全て解放
		m_stage->Uninit();	// 終了
		delete m_stage;
		m_stage = nullptr;
	}

	m_stage = new CStage;
	m_stage->Init(m_stageInfo[m_stageIndex]);	// 初期化
}
