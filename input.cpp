//=========================================
// 
// インプット処理
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input.h"

//=========================================
// 静的メンバー変数の初期化
//=========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//-----------------------------------------
// 初期化
//-----------------------------------------
CInput::CInput() : 
	m_pDevise(nullptr)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CInput::~CInput()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CInput::Uninit()
{
	// DirectInputオブジェクトの破壊
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
