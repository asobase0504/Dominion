//=========================================
// 
// 入力処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_XINPUT_H_
#define _INPUT_XINPUT_H_

#define DIRECTINPUT_VERSION 0x0800
#include "xinput.h"						// ジョイパッド処理に必要
#pragma comment(lib,"xinput.lib")		//入力処理に必要（ジョイパット）

#include <d3dx9.h>

class CXInput
{
public:
	//キー種類(ジョイパッド)
	enum JOYKEY
	{
		JOYKEY_CROSS_UP = 0,		//十字ボタン上
		JOYKEY_CROSS_DOWN,			//十字ボタン下
		JOYKEY_CROSS_LEFT,			//十字ボタン左
		JOYKEY_CROSS_RIGHT,			//十字ボタン右
		JOYKEY_START,				//スタートボタン
		JOYKEY_BACK,				//バックボタン
		JOYKEY_LEFT_THUMB,			//左スティック押込み
		JOYKEY_RIGHT_THUMB,			//右スティック押込み
		JOYKEY_LEFT_SHOULDER,		//L１ボタン
		JOYKEY_RIGHT_SHOULDER,		//R１ボタン
		JOYKEY_LEFT_TRIGGER,		//L２トリガー
		JOYKEY_RIGHT_TRIGGER,		//R２トリガー
		JOYKEY_A,					//Aボタン
		JOYKEY_B,					//Bボタン
		JOYKEY_X,					//Xボタン
		JOYKEY_Y,					//Yボタン
		JOYKEY_STICK_UP = 0,		//上	(8方向スティック)
		JOYKEY_STICK_UP_RIGHT,		//右上	(8方向スティック)
		JOYKEY_STICK_RIGHT,			//右	(8方向スティック)
		JOYKEY_STICK_DOWN_RIGHT,	//右下	(8方向スティック)
		JOYKEY_STICK_DOWN,			//下	(8方向スティック)
		JOYKEY_STICK_DOWN_LEFT,		//左下	(8方向スティック)
		JOYKEY_STICK_LEFT,			//左	(8方向スティック)
		JOYKEY_STICK_UP_LEFT,		//左上	(8方向スティック)
		JOYKEY_MAX
	};

	//スティック識別(ジョイパッド)
	enum JOYKEY_RIGHT_LEFT
	{
		JOYKEY_RIGHT_STICK = 0,
		JOYKEY_LEFT_STICK,
		JOYKEY_RIGHT_LEFT_MAX
	};

	//移動系のキーまとめ
	enum MOVE_KEY
	{
		MOVE_KEY_UP = 0,		//上キー
		MOVE_KEY_DOWN,			//下キー
		MOVE_KEY_RIGHT,			//右キー
		MOVE_KEY_LEFT,			//左キー
		MOVE_KEY_MAX
	};

	//キータイプ
	enum FUNCTION_KEY
	{
		FUNCTION_KEY_DESISION = 0,		//決定キー
		FUNCTION_KEY_PAUSE,				//ポーズキー
		FUNCTION_KEY_MAX
	};

	//ライトの色選択キー
	enum LIGHT_KEY
	{
		LIGHT_KEY_RED = 0,			//赤キー
		LIGHT_KEY_GREEN,			//緑キー
		LIGHT_KEY_BLUE,				//青キー
		LIGHT_KEY_YELLOW,			//黄キー
		LIGHT_KEY_MAX
	};

public:
	CXInput();
	virtual ~CXInput();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	bool GetJoypadPress(JOYKEY Key);										//プレス処理
	bool GetJoypadTrigger(JOYKEY Key);										//トリガー処理
	D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft);				//スティック処理
	int GetJoypadTriggerPedal(JOYKEY Key);									//トリガーペダル処理
	bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key);		//スティックプレス８方向
	bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key);	//スティックトリガー８方向			

protected:
	XINPUT_STATE m_JoyKeyState;				//ジョイパットのプレス情報
	XINPUT_STATE m_JoyKeyStateTrigger;		//ジョイパットのトリガー情報
	D3DXVECTOR3 m_JoyStickPos;				//ジョイスティックの傾き
	JOYKEY m_OldJoyKeyStick[JOYKEY_RIGHT_LEFT_MAX];		//前回のスティックの位置
	FUNCTION_KEY m_OldFunctionKey;						//前回の機能キーの情報
	LIGHT_KEY m_OldLightKey = LIGHT_KEY_MAX;			//前回のライトのキーの情報
	MOVE_KEY m_OldMoveKey;

	int m_nKeyCnt;
};

#endif // !_INPUT_XINPUT_H_
