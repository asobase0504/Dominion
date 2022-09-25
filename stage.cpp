#include "stage.h"
#include "application.h"
#include "game.h"
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"
#include "ui_countdown.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CStage::CStage() :
	map(nullptr),
	m_isEndGame(false),
	m_isPopFlag(false),
	m_PreparingCount(0)
{
	character.clear();
	controller.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CStage::~CStage()
{
	assert(character.empty());
	assert(controller.empty());
	assert(map == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CStage::Init(nlohmann::json inStage)
{
	// マップクラス
	stage = inStage;

	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	m_isPopFlag = true;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CStage::Uninit()
{
	DeleteBullet();

	// キャラクターの解放
	for (auto it = character.begin(); it != character.end();)
	{
		it = character.erase(it);
	}

	// コントローラーの解放
	for (auto it = controller.begin(); it != controller.end();)
	{
		// コントローラーの削除
		(*it)->Uninit();
		delete (*it);
		(*it) = nullptr;

		it = controller.erase(it);
	}

	// マップの解放
	if (map != nullptr)
	{
		map->Uninit();
		delete map;
		map = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CStage::Update()
{
	// 一度しか通らないプレイヤーの作成
	PassOnceCreatePlater();

	// キャラクターが死亡したかを調べる
	for (int i = 0; i < (int)character.size(); i++)
	{
		if (!character[i]->GetIsDeleted())
		{
			continue;
		}

		/* ↓死亡処理↓ */
		m_isEndGame = true;
	}

	if (m_isEndGame)
	{
		// 死んでないキャラクターを調べる
		for (int i = 0; i < (int)character.size(); i++)
		{
			character[i]->SetOperationState(false);
			if (character[i]->GetIsDeleted())
			{
				continue;
			}

			m_winnarTeam = character[i]->GetTeam();	// キャラクターのIdxを取得
		}
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CStage::Draw()
{
}

//-----------------------------------------------------------------------------
// 一度しか通らない処理
//-----------------------------------------------------------------------------
void CStage::PassOnceCreatePlater()
{
	if (!m_isPopFlag)
	{
		return;
	}

	if (m_PreparingCount <= 5)
	{
		m_PreparingCount++;
		return;
	}

	/* ↓カウントダウンが終了した場合↓ */

	// プレイヤーをラムダ式でクリエイト
	auto playerSet = [this](int inIdx, CController* inController)->void
	{
		controller.push_back(inController);
		int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// Xの位置番号を取得
		int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Yの位置番号を取得
		CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// チームの作成
		character.push_back(CCharacter::Create(inTeam));				// 生成
		float size = map->GetBlockSize() * 0.65f;						// 大きさの設定
		character.at(inIdx)->SetSize(D3DXVECTOR2(size, size));			// 大きさの代入
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// 位置の取得
		character.at(inIdx)->SetPos(pos);								// 位置の設定
		character.at(inIdx)->SetCenterBlockIndex({ x,y });				// 中央位置の設定
		character.at(inIdx)->SetBlockIndex(0, { x,y });					// 所属ブロックを設定
		character.at(inIdx)->SetController(inController);				// 命令者の設定
	};

	CGame* gameMode = (CGame*)CApplication::GetInstance()->GetMode();
	std::vector<int> charcterIndex = gameMode->GetControllerIndex();

	// キャラクターの設定
	for (int i = 0; i < (int)charcterIndex.size(); i++)
	{
		if (charcterIndex[i] != -2)
		{
			CPlayerController* incontroller = new CPlayerController(charcterIndex[i]);
			playerSet(i, incontroller);
			incontroller->Init();
		}
		else
		{
			CAIController* incontroller = new CAIController;
			playerSet(i, incontroller);
			incontroller->Init();
		}
	}

	m_isPopFlag = false;
}

//-----------------------------------------------------------------------------
// 弾の削除依頼
//-----------------------------------------------------------------------------
void CStage::DeleteBullet()
{
	// オブジェクト全体を取得
	for (auto it = CObject::GetMyObject(3)->begin(); it != CObject::GetMyObject(3)->end(); it++)
	{
		if ((*it)->CObject::GetType() == CObject::TYPE::BULLET)
		{
			(*it)->SetIsDeleted(true);
		}
	}
}

//-----------------------------------------------------------------------------
// ステージ内全ての削除依頼
//-----------------------------------------------------------------------------
void CStage::AllDelete()
{
	for (auto it = character.begin(); it != character.end();)
	{
		(*it)->SetIsDeleted(true);
		it = character.erase(it);
	}
	map->Delete();
}

//-----------------------------------------------------------------------------
// ラウンド勝者の番号を取得
//-----------------------------------------------------------------------------
int CStage::GetWinnerIndex()
{
	if (!m_isEndGame)
	{
		// まだゲームが終わってない場合
		assert(false);
		return -1;
	}

	return m_winnarTeam;
}
