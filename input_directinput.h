//=========================================
// 
// キーボード入力処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_DIRECTINPUT_H_
#define _INPUT_DIRECTINPUT_H_

#include "input.h"
#include <d3dx9.h>

class CDirectInput : public CInput
{
public:
	//キー種類
	enum JOYKEY
	{								//	PS5				スイッチpor				スイッチジョイコン（未調査）
		JOYKEY_DIRECT_0_BUTTON = 0,	//□ボタン			//Bボタン				//A
		JOYKEY_DIRECT_1_BUTTON,		//×ボタン			//Aボタン				//X
		JOYKEY_DIRECT_2_BUTTON,		//〇ボタン			//Yボタン				//B
		JOYKEY_DIRECT_3_BUTTON,		//△ボタン			//Xボタン				//Y
		JOYKEY_DIRECT_4_BUTTON,		//L1ボタン			//L1ボタン				//SL
		JOYKEY_DIRECT_5_BUTTON,		//R1ボタン			//R1ボタン				//SR
		JOYKEY_DIRECT_6_BUTTON,		//L2ボタン			//L2ボタン				//
		JOYKEY_DIRECT_7_BUTTON,		//R2ボタン			//R2ボタン				//
		JOYKEY_DIRECT_8_BUTTON,		//クリエイト		//-ボタン				//
		JOYKEY_DIRECT_9_BUTTON,		//オプション		//+ボタン				//
		JOYKEY_DIRECT_10_BUTTON,	//L3ボタン			//L3ボタン				//
		JOYKEY_DIRECT_11_BUTTON,	//R3ボタン			//R3ボタン				//
		JOYKEY_DIRECT_12_BUTTON,	//ホームボタン		//ホームボタン			//
		JOYKEY_DIRECT_13_BUTTON,	//タッチパッド		//キャプチャー			//
		JOYKEY_DIRECT_14_BUTTON,	//		//
		JOYKEY_DIRECT_15_BUTTON,	//		//
		JOYKEY_DIRECT_16_BUTTON,	//		//
		JOYKEY_DIRECT_17_BUTTON,	//左スティック		//
		JOYKEY_DIRECT_18_BUTTON,	//右スティック		//
		JOYKEY_DIRECT_19_BUTTON,	//十字キー（上）
		JOYKEY_DIRECT_20_BUTTON,	//十字キー（下）
		JOYKEY_DIRECT_21_BUTTON,	//十字キー（左）
		JOYKEY_DIRECT_22_BUTTON,	//十字キー（右）
		JOYKEY_DIRECT_MAX
	};

	//キー種類(十字キー&8方向スティック)
	enum JOYKEY_CROSS
	{
		JOYKEY_CROSS_UP = 0,								//上
		JOYKEY_CROSS_UP_RIGHT = 4500,					//右上
		JOYKEY_CROSS_RIGHT = 9000,						//右
		JOYKEY_CROSS_DOWN_RIGHT = 13500,					//右下
		JOYKEY_CROSS_DOWN = 18000,						//下
		JOYKEY_CROSS_DOWN_LEFT = 22500,					//左下
		JOYKEY_CROSS_LEFT = 27000,						//左
		JOYKEY_CROSS_UP_LEFT = 31500,					//左上
		JOYKEY_CROSS_MAX
	};

	//スティック識別(ジョイパッド)
	enum JOYKEY_RIGHT_LEFT
	{
		JOYKEY_RIGHT_STICK = 0,
		JOYKEY_LEFT_STICK,
		JOYKEY_RIGHT_LEFT_MAX
	};

public:
	CDirectInput();
	~CDirectInput() override;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(JOYKEY nKey);
	bool GetPress(JOYKEY_CROSS nKey);
	bool GetTrigger(JOYKEY nKey);
	bool GetTrigger(JOYKEY_CROSS nKey);
	bool GetRelease(JOYKEY nKey);
	bool GetRelease(JOYKEY_CROSS nKey);
	D3DXVECTOR3 GetStick(JOYKEY_RIGHT_LEFT Key);
	bool GetStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);
	bool GetStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);

private:
	DIJOYSTATE2 m_PressState;				// ジョイパット（プレス処理）
	DIJOYSTATE2 m_TriggerState;				// ジョイパット（トリガー処理）
	DIJOYSTATE2 m_ReleaseState;				// ジョイパット（リリース処理）
	DWORD m_OldJoyKeyDirect = 0;			// 前回の十字キーの値
	JOYKEY_CROSS m_OldJoyKeyStickDirect;	// 前回のスティックの位置
	bool m_bJoyKey = false;					// 使っているかどうか
};

#endif // !_INPUT_DIRECTINPUT_H_