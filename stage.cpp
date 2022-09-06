#include "stage.h"
#include "application.h"
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
	m_isPreparing(false),
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
HRESULT CStage::Init()
{
	// マップクラス
	stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	m_isPreparing = true;
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
	if (m_isPreparing)
	{
		if (m_PreparingCount <= CCountDownUI::READY_TIME + CCountDownUI::GO_TIME)
		{
			m_PreparingCount++;
			return;
		}

		// プレイヤーをラムダ式でクリエイト
		std::function<void(int, CController*)> playerSet;

		playerSet = [this](int inIdx, CController* inController)->void
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

		// キャラクターの設定
		playerSet(0, new CAIController);
		playerSet(1, new CPlayerController);
		m_isPreparing = false;
	}

	for (int i = 0; i < character.size(); i++)
	{
		if (character[i]->GetIsDeleted())
		{
			// 死亡時;
			m_isEndGame = true;
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
// 弾の削除依頼
//-----------------------------------------------------------------------------
void CStage::DeleteBullet()
{
	// オブジェクト全体を取得
	for (auto it = CObject::GetMyObject(2)->begin(); it != CObject::GetMyObject(2)->end(); it++)
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
