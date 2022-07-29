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
#include "game.h"

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
	object(nullptr)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CApplication::~CApplication()
{
	assert(renderer == nullptr);
	assert(input == nullptr);
	assert(directInput == nullptr);
	assert(texture == nullptr);
	assert(object == nullptr);
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

	// ゲームモード
	game = new CGame;
	if (FAILED(game->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CApplication::Uninit()
{
	// オブジェクト全体の解放
	CObject::ReleaseAll();

	// ゲームのクリア
	if (game != nullptr)
	{
		game = nullptr;
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
	assert(application == nullptr);
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
