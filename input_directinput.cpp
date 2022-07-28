//=========================================
// 
// �C���v�b�g����
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input_directinput.h"
#pragma comment(lib,"dxguid.lib")

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CDirectInput::CDirectInput()
{
	//memset(m_aKeyState, 0, sizeof(m_aKeyState));
	//memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	//memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CDirectInput::~CDirectInput()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X�i�W���C�p�b�h(DirectInput)�j�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevise, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevise->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevise->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h(DirectInput)�ւ̃A�N�Z�X�����l��
	m_pDevise->Acquire();

	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CDirectInput::Uninit()
{
	CInput::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CDirectInput::Update()
{
	DIJOYSTATE2 aKeyState;		//�W���C�p�b�h(DirectInput)�̓��͏��

	if (true)
	{
		//���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDevise->GetDeviceState(sizeof(aKeyState), &aKeyState)))
		{
			for (int nCnt = 0; nCnt < 32; nCnt++)
			{
				m_TriggerState.rgbButtons[nCnt] = (m_PressState.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & aKeyState.rgbButtons[nCnt];		//�L�[�{�[�h�̃g���K�[����ۑ�
				m_TriggerState.rgbButtons[nCnt] = (m_PressState.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & m_PressState.rgbButtons[nCnt];		//�L�[�{�[�h�̃����[�X����ۑ�
			}
			m_PressState = aKeyState;		//�v���X����
		}
		else
		{
			m_pDevise->Acquire();			//�W���C�p�b�h(DirectInput)�ւ̃A�N�Z�X�����l��
		}
	}
}

//-----------------------------------------
// �v���X����
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
	{//�g�p���Ă�����
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
	//�����AX�͉E���P�A����-�P�AY�͏オ�P�A����-�P

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
