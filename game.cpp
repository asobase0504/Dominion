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
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"
#include <assert.h>

#include "file.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGame::CGame() :
	//character(nullptr),
	map(nullptr)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(character.empty());
	assert(map == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	nlohmann::json stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	// マップクラス
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	// キャラクターの設定
	{
		int x = stage["PLAYERS"].at(0)["SPAWN"].at(0);
		int y = stage["PLAYERS"].at(0)["SPAWN"].at(1);
		character.push_back(CCharacter::Create(stage["PLAYERS"].at(0)["TYPE"], x));	// 生成
		character.at(0)->SetSize(D3DXVECTOR2(20.0f, 20.0f));			// 大きさの設定
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();
		character.at(0)->SetPos(pos);
		character.at(0)->SetController(new CPlayerController);				// 命令者の設定
	}

	// キャラクターの設定
	{
		int x = stage["PLAYERS"].at(1)["SPAWN"].at(0);
		int y = stage["PLAYERS"].at(1)["SPAWN"].at(1);
		character.push_back(CCharacter::Create(stage["PLAYERS"].at(1)["TYPE"],x));	// 生成
		character.at(1)->SetSize(D3DXVECTOR2(20.0f, 20.0f));						// 大きさの設定
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();
		character.at(1)->SetPos(pos);
		character.at(1)->SetController(new CPlayerController);				// 命令者の設定
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	for (auto it = character.begin(); it != character.end();)
	{
		(*it)->Uninit();
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGame::Update()
{
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}
