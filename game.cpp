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
#include <functional>

#include "application.h"
#include "input.h"
#include "input_keybord.h"

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
	stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	// 背景の設定
	{
		CObject2D* bg = CObject2D::Create();
		bg->SetSize(D3DXVECTOR2(CApplication::GetInstance()->SCREEN_WIDTH, CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->SCREEN_WIDTH * 0.5f, CApplication::GetInstance()->SCREEN_HEIGHT * 0.5f, 0.0f);	// 位置の取得
		bg->SetTexture(CTexture::TEXTURE::TEXTERE_BG);
		bg->SetPos(pos);
		bg->SetColor(D3DXCOLOR(0.45f, 0.45f, 0.9f, 1.0f));
	}

	// マップクラス
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	// プレイヤーをラムダ式でクリエイト
	std::function<void(int, CController*)> playerSet;

	playerSet = [this](int inIdx, CController* inController)->void
	{
		int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// Xの位置番号を取得
		int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Yの位置番号を取得
		CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// チームの作成
		character.push_back(CCharacter::Create(inTeam));				// 生成
		character.at(inIdx)->SetSize(D3DXVECTOR2(25.0f, 25.0f));		// 大きさの設定
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// 位置の取得
		character.at(inIdx)->SetPos(pos);								// 位置の設定
		character.at(inIdx)->SetBlockIndex(0, { x,y });					// 所属ブロックを設定
		character.at(inIdx)->SetController(inController);				// 命令者の設定
	};

	// キャラクターの設定
	playerSet(0, new CAIController);
	playerSet(1, new CPlayerController);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	for (auto it = character.begin(); it != character.end();)
	{
		it = character.erase(it);
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
void CGame::Update()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	//	画面の遷移
	if (input->GetTrigger(DIK_RETURN))
	{
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}
