//=========================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include <assert.h>
#include "input_keybord.h"
#include "input_directinput.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"

//-----------------------------------------------------------------------------
// 静的メンバー変数の初期化
//-----------------------------------------------------------------------------
CApplication* CApplication::application = nullptr;

//=============================================================================
// シングルトンでのインスタンスの取得
//=============================================================================
CApplication* CApplication::GetInstance()
{
	if (application == nullptr)
	{
		application = new CApplication;
	}

	return application;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CApplication::CApplication() :
	renderer(nullptr),
	input(nullptr),
	directInput(nullptr),
	texture(nullptr),
	object(nullptr),
	character(nullptr),
	map(nullptr)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CApplication::~CApplication()
{
	assert(object == nullptr);
	assert(renderer == nullptr);
	assert(character == nullptr);
	assert(input == nullptr);
	assert(texture == nullptr);
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
	// 入力処理
	input = new CInputKeybord;
	if (FAILED(input->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//directInput = new CDirectInput;
	//if (FAILED(directInput->Init(hInstance, hWnd)))
	//{
	//	return E_FAIL;
	//}

	// レンダリングクラス
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	// テクスチャ
	texture = new CTexture;
	texture->LoadAll();

	// マップクラス
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->Set();

	// プレイヤークラス
	character = CCharacter::Create(CCharacter::TEAM_00);	// 生成
	character->SetPos(D3DXVECTOR3(800.0f, 300.0f, 0.0f));	// 位置の設定
	character->SetSize(D3DXVECTOR2(20.0f, 20.0f));			// 大きさの設定
	character->SetController(new CPlayerController);		// 命令者の設定

	character = CCharacter::Create(CCharacter::TEAM_01);	// 生成
	character->SetPos(D3DXVECTOR3(200.0f, 300.0f, 0.0f));	// 位置の設定
	character->SetSize(D3DXVECTOR2(20.0f, 20.0f));			// 大きさの設定
	character->SetController(new CPlayerController);		// 命令者の設定

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CApplication::Uninit()
{
	// オブジェクト全体の解放
	CObject::ReleaseAll();

	// プレイヤーのクリア
	if (character != nullptr)
	{
		character = nullptr;
	}

	// テクスチャの解放
	texture->UnloadAll();
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}

	// 入力処理の解放
	if (input != nullptr)
	{
		input->Uninit();

		delete input;
		input = nullptr;
	}

	// 入力処理の解放
	if (directInput != nullptr)
	{
		directInput->Uninit();

		delete directInput;
		directInput = nullptr;
	}

	// レンダラーの解放
	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	// アプリケーションの解放
	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CApplication::Update()
{
	input->Update();
	//directInput->Update();
	renderer->Update();
}

//=============================================================================
// 描画
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}
