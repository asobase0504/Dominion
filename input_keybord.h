//=========================================
// 
// キーボード入力処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_KETBORD_H_
#define _INPUT_KETBORD_H_

#include "input.h"

class CInputKeybord : public CInput
{
public:
	CInputKeybord();
	~CInputKeybord() override;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
	BYTE m_aKeyStateRelease[256];
};
#endif // !_INPUT_KETBORD_H_
