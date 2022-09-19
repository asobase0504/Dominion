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
#include "input.h"
#include "renderer.h"
#include "theme_color.h"
#include "texture.h"
#include "object2d.h"
#include "title.h"
#include "game.h"
#include "customize.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// 静的メンバー変数の初期化
//-----------------------------------------------------------------------------
CApplication* CApplication::application = nullptr;

//-----------------------------------------------------------------------------
// シングルトンでのインスタンスの取得
//-----------------------------------------------------------------------------
CApplication* CApplication::GetInstance()
{
	if (application == nullptr)
	{
		application = new CApplication;
	}

	return application;
}

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CApplication::CApplication() :
	SCREEN_WIDTH(1280),
	SCREEN_HEIGHT(720),
	CENTER_X(SCREEN_WIDTH * 0.5f),
	CENTER_Y(SCREEN_HEIGHT * 0.5f),
	mode(nullptr),
	renderer(nullptr),
	input(nullptr),
	texture(nullptr),
	object(nullptr),
	color(nullptr)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CApplication::~CApplication()
{
	assert(mode == nullptr);
	assert(renderer == nullptr);
	assert(input == nullptr);
	assert(texture == nullptr);
	assert(object == nullptr);
	assert(color == nullptr);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
	// 入力処理
	input = CInput::Create();
	if (FAILED(input->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// レンダリングクラス
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	// テクスチャ
	texture = new CTexture;
	texture->LoadAll();

	// 色管理クラス
	color = new CThemeColor;
	if (FAILED(color->Init()))
	{
		return E_FAIL;
	}

	// サウンドクラス
	sound = new CSound;
	if (FAILED(sound->Init(hWnd)))
	{
		return E_FAIL;
	}
	sound->Play(CSound::LABEL_BGM_TITLE);

	// ゲームモード
	SetMode(MODE_TYPE::TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CApplication::Uninit()
{
	// オブジェクト全体の解放
	CObject::ReleaseAll();

	// ゲームのクリア
	if (mode != nullptr)
	{
		mode->Uninit();

		delete mode;
		mode = nullptr;
	}

	// サウンドの解放
	if (sound != nullptr)
	{
		sound->Uninit();
		delete sound;
		sound = nullptr;
	}

	// テクスチャの解放
	texture->UnloadAll();
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}

	// 色管理の解放
	if (color != nullptr)
	{
		delete color;
		color = nullptr;
	}

	// 入力処理の解放
	if (input != nullptr)
	{
		input->Uninit();
		input = nullptr;
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

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CApplication::Update()
{
	input->Update();
	renderer->Update();
	mode->Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CApplication::Draw()
{
	renderer->Draw();
}

//-----------------------------------------------------------------------------
// 色の取得
//-----------------------------------------------------------------------------
D3DXCOLOR CApplication::GetColor(int inKey)
{
	return color->GetColor(inKey);
}

int CApplication::GetColorSize()
{
	return color->GetSize();
}

//-----------------------------------------------------------------------------
// モードの設定
//-----------------------------------------------------------------------------
void CApplication::SetMode(MODE_TYPE inType)
{
	if (mode != nullptr)
	{
		CObject::ReleaseAll();
		mode->Uninit();
		delete mode;
		mode = nullptr;
	}

	switch (inType)
	{
	case CApplication::MODE_TYPE::TITLE:
		mode = new CTitle;
		break;
	case CApplication::MODE_TYPE::CUSTUM:
		mode = new CCustomize;
		break;
	case CApplication::MODE_TYPE::GAME:
		mode = new CGame;
		break;
	case CApplication::MODE_TYPE::RESULT:
		break;
	default:
		break;
	}

	if (FAILED(mode->Init()))
	{
		assert(false);
	}
}

//-----------------------------------------------------------------------------
// テーマカラーの設定
//-----------------------------------------------------------------------------
void CApplication::SetThemeColor(int idx)
{
	color->SetTheme(idx);	// テーマ色の設定
}
