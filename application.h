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
#include <string>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CRenderer;
class CObject;
class CInputKeybord;
class CDirectInput;
class CTexture;
class CMode;
class CColor;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CApplication
{
public:
	enum class MODE_TYPE
	{
		TITLE = 0,
		GAME,
		RESULT,
		MAX
	};

public: // 定数
	const int SCREEN_WIDTH;		// スクリーンの幅
	const int SCREEN_HEIGHT;	// スクリーンの高さ
	const float CENTER_X;			// スクリーン中心座標のX座標
	const float CENTER_Y;			// スクリーン中心座標のY座標

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

	// Getter
	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CInputKeybord* GetInput() { return input; }
	CDirectInput* GetDirectInput() { return directInput; }
	CTexture* GetTextureClass() { return texture; }
	CMode* GetMode() { return mode; }
	D3DXCOLOR GetColor(std::string inKey);

	// Setter
	void SetMode(MODE_TYPE inType);

private: // 動的メンバー変数
	CMode* mode;
	CRenderer* renderer;
	CInputKeybord* input;
	CDirectInput* directInput;
	CTexture* texture;
	CObject* object;
	CColor* color;
};

#endif // !_APPLICATION_H_
