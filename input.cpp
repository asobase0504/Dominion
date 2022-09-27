//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : YUDA KAITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"

//-----------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//このクラスの情報

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//キーボードの情報
	m_pJoyPad = nullptr;		//ジョイパッドの情報
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの生成
	m_pJoyPad = new CInputJoyPad;

	//ジョイパッドの初期化処理
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInput::Uninit()
{
	//キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//ジョイパッドの破棄
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//DirectInputオブジェクトの破棄
	CDirectInput::Break();

	//自己破棄
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInput::Update()
{
	//キーボードの更新
	m_pKeyboard->Update();
	//ジョイパッドの更新
	m_pJoyPad->Update();
}

//*************************************************************************************
//インプットの生成
//*************************************************************************************
CInput *CInput::Create()
{
	//Inputの自己生成
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//プレス処理(総合)
//*************************************************************************************
bool CInput::Press(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardPress(DIK_W)
			|| m_pKeyboard->GetKeyboardPress(DIK_UP)
			|| m_pJoyPad->GetPressAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardPress(DIK_S)
			|| m_pKeyboard->GetKeyboardPress(DIK_DOWN)
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_A)
			|| m_pKeyboard->GetKeyboardPress(DIK_LEFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardPress(DIK_D)
			|| m_pKeyboard->GetKeyboardPress(DIK_RIGHT)
			|| m_pJoyPad->GetPressAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardPress(DIK_RETURN)
			|| m_pJoyPad->GetPressAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardPress(DIK_SPACE)
			|| m_pJoyPad->GetPressAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardPress(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardPress(DIK_B)
			|| m_pJoyPad->GetPressAll(JOYPAD_BACK)
			|| m_pJoyPad->GetPressAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardPress(DIK_LSHIFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_L1))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Press(KEY_UP) || Press(KEY_DOWN) || Press(KEY_LEFT) || Press(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardPress(DIK_P)
			|| m_pJoyPad->GetPressAll(JOYPAD_START))
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//トリガー処理(総合)
//*************************************************************************************
bool CInput::Trigger(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_W)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_UP)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_S)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_DOWN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_A)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LEFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_D)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RETURN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_SPACE)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_B)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_BACK)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LSHIFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_P)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_START)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_HOME))
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//リリース処理(総合)
//*************************************************************************************
bool CInput::Release(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardRelease(DIK_W)
			|| m_pKeyboard->GetKeyboardRelease(DIK_UP)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardRelease(DIK_S)
			|| m_pKeyboard->GetKeyboardRelease(DIK_DOWN)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardRelease(DIK_A)
			|| m_pKeyboard->GetKeyboardRelease(DIK_LEFT)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardRelease(DIK_D)
			|| m_pKeyboard->GetKeyboardRelease(DIK_RIGHT)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardRelease(DIK_RETURN)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardRelease(DIK_SPACE)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardRelease(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardRelease(DIK_B)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_BACK)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardRelease(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardRelease(DIK_LSHIFT)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Release(KEY_UP) || Release(KEY_DOWN) || Release(KEY_LEFT) || Release(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardRelease(DIK_P)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_START)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_HOME))
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool CInput::Press(STAN_DART_INPUT_KEY key, int nNum)
{
	switch (key)
	{
	case KEY_UP:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardPress(DIK_W) || m_pKeyboard->GetKeyboardPress(DIK_UP);
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_UP, nNum);
		}

		break;
	case KEY_DOWN:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardPress(DIK_S) || m_pKeyboard->GetKeyboardPress(DIK_DOWN);
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_DOWN, nNum);
		}
		break;
	case KEY_LEFT:
		if (nNum == -1)
		{
			if ((m_pKeyboard->GetKeyboardPress(DIK_A)
				|| m_pKeyboard->GetKeyboardPress(DIK_LEFT)))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_LEFT, nNum);
		}
		break;
	case KEY_RIGHT:
		if (nNum == -1)
		{
			if (m_pKeyboard->GetKeyboardPress(DIK_D)
				|| m_pKeyboard->GetKeyboardPress(DIK_RIGHT))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_RIGHT, nNum);
		}
		break;
	case KEY_MOVE_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_W) : m_pJoyPad->GetPress(JOYPAD_UP, nNum);
		break;
	case KEY_MOVE_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_S) : m_pJoyPad->GetPress(JOYPAD_DOWN, nNum);
		break;
	case KEY_MOVE_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_A) : m_pJoyPad->GetPress(JOYPAD_LEFT, nNum);
		break;
	case KEY_MOVE_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_D) : m_pJoyPad->GetPress(JOYPAD_RIGHT, nNum);
		break;
	case KEY_SHOT_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_UP) : m_pJoyPad->GetPress(JOYPAD_Y, nNum);
		break;
	case KEY_SHOT_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_DOWN) : m_pJoyPad->GetPress(JOYPAD_A, nNum);
		break;
	case KEY_SHOT_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_LEFT) : m_pJoyPad->GetPress(JOYPAD_X, nNum);
		break;
	case KEY_SHOT_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardPress(DIK_RIGHT) : m_pJoyPad->GetPress(JOYPAD_B, nNum);
		break;
	case KEY_DECISION:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardPress(DIK_RETURN);
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_A, nNum);
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardPress(DIK_SPACE)
			|| m_pJoyPad->GetPressAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardPress(DIK_BACKSPACE) || m_pKeyboard->GetKeyboardPress(DIK_B);
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_BACK, nNum) || m_pJoyPad->GetPress(JOYPAD_B, nNum);
		}
		break;
	case KEY_MOVE:
		if (Press(KEY_UP, nNum) || Press(KEY_DOWN, nNum) || Press(KEY_LEFT, nNum) || Press(KEY_RIGHT, nNum))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardPress(DIK_P);
		}
		else
		{
			return m_pJoyPad->GetPress(JOYPAD_START, nNum) || m_pJoyPad->GetPress(JOYPAD_HOME, nNum);
		}
		break;
	default:
		break;
	}

	return false;
}

bool CInput::Trigger(STAN_DART_INPUT_KEY key, int nNum)
{
	switch (key)
	{
	case KEY_UP:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardTrigger(DIK_W) || m_pKeyboard->GetKeyboardTrigger(DIK_UP);
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_UP, nNum);
		}

		break;
	case KEY_DOWN:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardTrigger(DIK_S) || m_pKeyboard->GetKeyboardTrigger(DIK_DOWN);
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_DOWN, nNum);
		}
		break;
	case KEY_LEFT:
		if (nNum == -1)
		{
			if ((m_pKeyboard->GetKeyboardTrigger(DIK_A)
				|| m_pKeyboard->GetKeyboardTrigger(DIK_LEFT)))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_LEFT, nNum);
		}
		break;
	case KEY_RIGHT:
		if (nNum == -1)
		{
			if (m_pKeyboard->GetKeyboardTrigger(DIK_D)
				|| m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_RIGHT, nNum);
		}
		break;
	case KEY_MOVE_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_W) : m_pJoyPad->GetTrigger(JOYPAD_UP, nNum);
		break;
	case KEY_MOVE_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_S) : m_pJoyPad->GetTrigger(JOYPAD_DOWN, nNum);
		break;
	case KEY_MOVE_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_A) : m_pJoyPad->GetTrigger(JOYPAD_LEFT, nNum);
		break;
	case KEY_MOVE_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_D) : m_pJoyPad->GetTrigger(JOYPAD_RIGHT, nNum);
		break;
	case KEY_SHOT_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_UP) : m_pJoyPad->GetTrigger(JOYPAD_Y, nNum);
		break;
	case KEY_SHOT_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_DOWN) : m_pJoyPad->GetTrigger(JOYPAD_A, nNum);
		break;
	case KEY_SHOT_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_LEFT) : m_pJoyPad->GetTrigger(JOYPAD_X, nNum);
		break;
	case KEY_SHOT_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT) : m_pJoyPad->GetTrigger(JOYPAD_B, nNum);
		break;
	case KEY_DECISION:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardTrigger(DIK_RETURN);
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_A, nNum);
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_SPACE)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardTrigger(DIK_BACKSPACE) || m_pKeyboard->GetKeyboardTrigger(DIK_B);
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_BACK, nNum) || m_pJoyPad->GetTrigger(JOYPAD_B, nNum);
		}
		break;
	case KEY_MOVE:
		if (Trigger(KEY_UP, nNum) || Trigger(KEY_DOWN, nNum) || Trigger(KEY_LEFT, nNum) || Trigger(KEY_RIGHT, nNum))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardTrigger(DIK_P);
		}
		else
		{
			return m_pJoyPad->GetTrigger(JOYPAD_START, nNum) || m_pJoyPad->GetTrigger(JOYPAD_HOME, nNum);
		}
		break;
	default:
		break;
	}

	return false;
}

bool CInput::Release(STAN_DART_INPUT_KEY key, int nNum)
{
	switch (key)
	{
	case KEY_UP:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardRelease(DIK_W) || m_pKeyboard->GetKeyboardRelease(DIK_UP);
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_UP, nNum);
		}

		break;
	case KEY_DOWN:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardRelease(DIK_S) || m_pKeyboard->GetKeyboardRelease(DIK_DOWN);
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_DOWN, nNum);
		}
		break;
	case KEY_LEFT:
		if (nNum == -1)
		{
			if ((m_pKeyboard->GetKeyboardRelease(DIK_A)
				|| m_pKeyboard->GetKeyboardRelease(DIK_LEFT)))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_LEFT, nNum);
		}
		break;
	case KEY_RIGHT:
		if (nNum == -1)
		{
			if (m_pKeyboard->GetKeyboardRelease(DIK_D)
				|| m_pKeyboard->GetKeyboardRelease(DIK_RIGHT))
			{
				return true;
			}
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_RIGHT, nNum);
		}
		break;
	case KEY_MOVE_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_W) : m_pJoyPad->GetRelease(JOYPAD_UP, nNum);
		break;
	case KEY_MOVE_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_S) : m_pJoyPad->GetRelease(JOYPAD_DOWN, nNum);
		break;
	case KEY_MOVE_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_A) : m_pJoyPad->GetRelease(JOYPAD_LEFT, nNum);
		break;
	case KEY_MOVE_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_D) : m_pJoyPad->GetRelease(JOYPAD_RIGHT, nNum);
		break;
	case KEY_SHOT_UP:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_UP) : m_pJoyPad->GetRelease(JOYPAD_Y, nNum);
		break;
	case KEY_SHOT_DOWN:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_DOWN) : m_pJoyPad->GetRelease(JOYPAD_A, nNum);
		break;
	case KEY_SHOT_LEFT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_LEFT) : m_pJoyPad->GetRelease(JOYPAD_X, nNum);
		break;
	case KEY_SHOT_RIGHT:
		return nNum == -1 ? m_pKeyboard->GetKeyboardRelease(DIK_RIGHT) : m_pJoyPad->GetRelease(JOYPAD_B, nNum);
		break;
	case KEY_DECISION:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardRelease(DIK_RETURN);
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_A, nNum);
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardRelease(DIK_SPACE)
			|| m_pJoyPad->GetReleaseAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardRelease(DIK_BACKSPACE) || m_pKeyboard->GetKeyboardRelease(DIK_B);
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_BACK, nNum) || m_pJoyPad->GetRelease(JOYPAD_B, nNum);
		}
		break;
	case KEY_MOVE:
		if (Release(KEY_UP, nNum) || Release(KEY_DOWN, nNum) || Release(KEY_LEFT, nNum) || Release(KEY_RIGHT, nNum))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (nNum == -1)
		{
			return m_pKeyboard->GetKeyboardRelease(DIK_P);
		}
		else
		{
			return m_pJoyPad->GetRelease(JOYPAD_START, nNum) || m_pJoyPad->GetRelease(JOYPAD_HOME, nNum);
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Press)
//*************************************************************************************
std::vector<int> CInput::PressDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0;i < m_pJoyPad->GetJoyPadNumMax();i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}
	
	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Trigger)
//*************************************************************************************
std::vector<int> CInput::TriggerDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Trigger(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Trigger(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Release)
//*************************************************************************************
std::vector<int> CInput::ReleaseDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
//プレス処理(キーボード)
//*************************************************************************************
bool CInput::Press(int nKey)
{
	return m_pKeyboard->GetKeyboardPress(nKey);
}

//*************************************************************************************
//トリガー処理(キーボード)
//*************************************************************************************
bool CInput::Trigger(int nkey)
{
	return m_pKeyboard->GetKeyboardTrigger(nkey);
}

//*************************************************************************************
//リリース処理(キーボード)
//*************************************************************************************
bool CInput::Release(int nkey)
{
	return m_pKeyboard->GetKeyboardRelease(nkey);
}

//*************************************************************************************
//プレス処理(ジョイパッド)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//トリガー処理(ジョイパッド)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//リリース処理(ジョイパッド)
//*************************************************************************************
bool CInput::Release(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetRelease(key, nNum);
}

//*************************************************************************************
//十字キーのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}
	int nCheck = m_pJoyPad->GetCross();

	if (nCheck > 360 || nCheck < 0)
	{//十字キーの角度が指定外だったら０を返す
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

int CInput::GetAcceptJoyPadCount()
{
	return m_pJoyPad->GetJoyPadNumMax();
}
