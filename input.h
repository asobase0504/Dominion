//=============================================================================
//
// 入力処理 [input.h]
// Author : KOZUNA HIROHITO
// Author2 : YUDA KAITO
// 
//=============================================================================

#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"
#include <vector>

//----------------------------------------------------------------------------
//前方宣言
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInput
{
public:

	CInput();
	~CInput();

	//*アプリケーション処理に書くやつ
	static CInput *Create();							//入力処理系のクリエイト、Initの前に書く
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
	void Uninit();										//入力処理全部の終了処理
	void Update();										//入力処理全部の更新処理
	//*

	//*インプットが必要な時呼び出す
	static CInput *GetKey() { return m_pInput; }		//プレイやトリガーなどのアドレスの取得

	// 全てのデバイス
	bool Press(STAN_DART_INPUT_KEY key);				//総合プレス
	bool Trigger(STAN_DART_INPUT_KEY key);				//総合トリガー
	bool Release(STAN_DART_INPUT_KEY key);				//総合リリース

	// 入力しているデバイスを指定
	bool Press(STAN_DART_INPUT_KEY key, int nNum);		//総合プレス
	bool Trigger(STAN_DART_INPUT_KEY key, int nNum);	//総合トリガー
	bool Release(STAN_DART_INPUT_KEY key, int nNum);	//総合リリース

	// 入力しているデバイスを返す
	std::vector<int> PressDevice(STAN_DART_INPUT_KEY key);		// 総合プレス
	std::vector<int> TriggerDevice(STAN_DART_INPUT_KEY key);	// 総合トリガー
	std::vector<int> ReleaseDevice(STAN_DART_INPUT_KEY key);	// 総合リリース

	bool Press(int nKey);								//キーボードプレス
	bool Trigger(int nkey);								//キーボードトリガー
	bool Release(int nkey);								//キーボードリリース
	bool Press(DirectJoypad key, int nNum = 0);			//ジョイパットプレス
	bool Trigger(DirectJoypad key, int nNum = 0);		//ジョイパットトリガー
	bool Release(DirectJoypad key, int nNum = 0);		//ジョイパッドリリース

	D3DXVECTOR3 VectorMoveKey();								//十字キー式のベクトル取得
	D3DXVECTOR3 VectorMoveJoyStick(int nNum = 0, bool bleftandright = false); //ジョイスティックのベクトル取得
	//*
	
	// Getter
	int GetAcceptJoyPadCount();
private:

	CInputKeyboard *m_pKeyboard;	//キーボードの情報
	CInputJoyPad *m_pJoyPad;		//ジョイパッドの情報
	static CInput *m_pInput;		//このクラスの情報
};

#endif

