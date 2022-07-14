//=========================================
// 
// ì¸óÕèàóùÉNÉâÉX
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#pragma comment(lib,"dinput8.lib")

class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	LPDIRECTINPUTDEVICE8 m_pDevise;
	static LPDIRECTINPUT8 m_pInput;
};

#endif // !_INPUT_H_
