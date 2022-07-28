//=========================================
// 
// インプット処理
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input_keybord.h"
#pragma comment(lib,"dxguid.lib")

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CInputKeybord::CInputKeybord()
{
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CInputKeybord::~CInputKeybord()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CInputKeybord::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevise, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevise->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevise->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevise->Acquire();

	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CInputKeybord::Uninit()
{
	CInput::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CInputKeybord::Update()
{
	BYTE aKeyState[256];		//キーボードの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevise->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int i = 0; i < 256; i++)
		{
			m_aKeyStateTrigger[i] = ~m_aKeyState[i] & aKeyState[i];	// キーボードのトリガー情報を保存
			m_aKeyStateRelease[i] = m_aKeyState[i] & ~aKeyState[i];	// キーボードのリリース情報を保存
			m_aKeyState[i] = aKeyState[i];							// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevise->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//-----------------------------------------
// プレス情報の取得
//-----------------------------------------
bool CInputKeybord::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) != 0;
}

//-----------------------------------------
// トリガー情報の取得
//-----------------------------------------
bool CInputKeybord::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//-----------------------------------------
// リリース情報の取得
//-----------------------------------------
bool CInputKeybord::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) != 0;
}
