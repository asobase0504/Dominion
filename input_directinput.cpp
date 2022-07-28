//=========================================
// 
// インプット処理
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input_directinput.h"
#pragma comment(lib,"dxguid.lib")

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CDirectInput::CDirectInput()
{
	//memset(m_aKeyState, 0, sizeof(m_aKeyState));
	//memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	//memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CDirectInput::~CDirectInput()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス（ジョイパッド(DirectInput)）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevise, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevise->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevise->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ジョイパッド(DirectInput)へのアクセス権を獲得
	m_pDevise->Acquire();

	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CDirectInput::Uninit()
{
	CInput::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CDirectInput::Update()
{
	DIJOYSTATE2 aKeyState;		//ジョイパッド(DirectInput)の入力情報

	if (true)
	{
		//入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevise->GetDeviceState(sizeof(aKeyState), &aKeyState)))
		{
			for (int nCnt = 0; nCnt < 32; nCnt++)
			{
				m_TriggerState.rgbButtons[nCnt] = (m_PressState.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & aKeyState.rgbButtons[nCnt];		//キーボードのトリガー情報を保存
				m_TriggerState.rgbButtons[nCnt] = (m_PressState.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & m_PressState.rgbButtons[nCnt];		//キーボードのリリース情報を保存
			}
			m_PressState = aKeyState;		//プレス処理
		}
		else
		{
			m_pDevise->Acquire();			//ジョイパッド(DirectInput)へのアクセス権を獲得
		}
	}
}

//-----------------------------------------
// プレス処理
//-----------------------------------------
bool CDirectInput::GetPress(JOYKEY nKey)
{
	return (m_PressState.rgbButtons[nKey] & 0x80) ? true : false;
}

bool CDirectInput::GetPress(JOYKEY_CROSS nKey)
{
	return m_PressState.rgdwPOV[0] == nKey;
}

bool CDirectInput::GetTrigger(JOYKEY nKey)
{
	return (m_TriggerState.rgbButtons[nKey] & 0x80) ? true : false;
}

bool CDirectInput::GetTrigger(JOYKEY_CROSS nKey)
{
	if (m_PressState.rgdwPOV[0] == m_OldJoyKeyDirect)
	{
		return false;
	}
	else if (m_PressState.rgdwPOV[0] == nKey
		&& m_PressState.rgdwPOV[0] != m_OldJoyKeyDirect)
	{
		m_OldJoyKeyDirect = m_PressState.rgdwPOV[0];
		return true;
	}
	m_OldJoyKeyDirect = 0xffffffff;
	return false;
}

bool CDirectInput::GetRelease(JOYKEY nKey)
{
	return (m_TriggerState.rgbButtons[nKey] & 0x80) ? true : false;
}

bool CDirectInput::GetRelease(JOYKEY_CROSS nKey)
{
	return m_TriggerState.rgdwPOV[0] == nKey;
}

D3DXVECTOR3 CDirectInput::GetStick(JOYKEY_RIGHT_LEFT Key)
{
	if (m_bJoyKey)
	{//使用していたら
		switch (Key)
		{
		case JOYKEY_RIGHT_STICK:
			return D3DXVECTOR3(float(m_PressState.lZ) / 32767.0f - 1.0f, -float(m_PressState.lRz) / 32767.0f + 1.0f, 0.0f);
			break;
		case JOYKEY_LEFT_STICK:
			return D3DXVECTOR3(float(m_PressState.lX) / 32767.0f - 1.0f, -float(m_PressState.lY) / 32767.0f + 1.0f, 0.0f);
			break;
		}
	}
	//メモ、Xは右が１、左が-１、Yは上が１、下が-１

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

bool CDirectInput::GetStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	D3DXVECTOR3 pos = GetStick(RightLeft);

	if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

bool CDirectInput::GetStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	if (GetStickPress(RightLeft, Key)
		&& Key != m_OldJoyKeyStickDirect)
	{
		m_OldJoyKeyStickDirect = Key;
		return true;
	}
	else if (GetStickPress(RightLeft, Key)
		&& Key == m_OldJoyKeyStickDirect)
	{
		return false;
	}

	m_OldJoyKeyStickDirect = JOYKEY_CROSS_MAX;
	return false;
}
