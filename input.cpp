//=============================================================================
//
// ���͏��� [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : YUDA KAITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"

//-----------------------------------------------------------------------------
//�ÓI�����o�ϐ��錾
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//���̃N���X�̏��

//*************************************************************************************
//�R���X�g���N�^
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//�L�[�{�[�h�̏��
	m_pJoyPad = nullptr;		//�W���C�p�b�h�̏��
}

//*************************************************************************************
//�f�X�g���N�^
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//������
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̐���
	m_pKeyboard = new CInputKeyboard;

	//�L�[�{�[�h�̏���������
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h�̐���
	m_pJoyPad = new CInputJoyPad;

	//�W���C�p�b�h�̏���������
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//�I������
//*************************************************************************************
void CInput::Uninit()
{
	//�L�[�{�[�h�̔j��
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//�W���C�p�b�h�̔j��
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	CDirectInput::Break();

	//���Ȕj��
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//�X�V����
//*************************************************************************************
void CInput::Update()
{
	//�L�[�{�[�h�̍X�V
	m_pKeyboard->Update();
	//�W���C�p�b�h�̍X�V
	m_pJoyPad->Update();
}

//*************************************************************************************
//�C���v�b�g�̐���
//*************************************************************************************
CInput *CInput::Create()
{
	//Input�̎��Ȑ���
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//�v���X����(����)
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
//�g���K�[����(����)
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
//�����[�X����(����)
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
// ���͂����f�o�C�X�̔ԍ����擾 (Press)
//*************************************************************************************
std::vector<int> CInput::PressDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// �L�[�{�[�h���͂̒���
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad���͂̒���
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
// ���͂����f�o�C�X�̔ԍ����擾 (Trigger)
//*************************************************************************************
std::vector<int> CInput::TriggerDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// �L�[�{�[�h���͂̒���
	if (Trigger(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad���͂̒���
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
// ���͂����f�o�C�X�̔ԍ����擾 (Release)
//*************************************************************************************
std::vector<int> CInput::ReleaseDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// �L�[�{�[�h���͂̒���
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad���͂̒���
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
//�v���X����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Press(int nKey)
{
	return m_pKeyboard->GetKeyboardPress(nKey);
}

//*************************************************************************************
//�g���K�[����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Trigger(int nkey)
{
	return m_pKeyboard->GetKeyboardTrigger(nkey);
}

//*************************************************************************************
//�����[�X����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Release(int nkey)
{
	return m_pKeyboard->GetKeyboardRelease(nkey);
}

//*************************************************************************************
//�v���X����(�W���C�p�b�h)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//�g���K�[����(�W���C�p�b�h)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//�����[�X����(�W���C�p�b�h)
//*************************************************************************************
bool CInput::Release(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetRelease(key, nNum);
}

//*************************************************************************************
//�\���L�[�̃x�N�g��
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
	{//�\���L�[�̊p�x���w��O��������O��Ԃ�
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//�����P�̃x�N�g���ϊ�
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

//*************************************************************************************
//�W���C�X�e�B�b�N�̃x�N�g��
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//�����P�̃x�N�g���ϊ�
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

int CInput::GetAcceptJoyPadCount()
{
	return m_pJoyPad->GetJoyPadNumMax();
}
