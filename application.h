//=========================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>
#include <vector>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CRenderer;
class CObject;
class CCharacter;
class CInputKeybord;
class CDirectInput;
class CTexture;
class CMap;
class CGame;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CApplication
{
public: // 定数
	const int SCREEN_WIDTH = 1280;	// スクリーンの幅
	const int SCREEN_HEIGHT = 720;	// スクリーンの高さ

public: // 静的メンバー変数
	static CApplication* application;
public: // 静的関数
	static CApplication* GetInstance();

private: // シングルトン
	CApplication();
public:	// メンバー関数
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CInputKeybord* GetInput() { return input; }
	CDirectInput* GetDirectInput() { return directInput; }
	CTexture* GetTextureClass() { return texture; }
	CGame* GetGame() { return game; }

private: // 動的メンバー変数
	CGame* game;
	CRenderer* renderer;
	CInputKeybord* input;
	CDirectInput* directInput;
	CTexture* texture;
	CObject* object;
};

#endif // !_APPLICATION_H_
