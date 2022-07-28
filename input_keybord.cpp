//=========================================
// 
// �C���v�b�g����
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input_keybord.h"
#pragma comment(lib,"dxguid.lib")

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CInputKeybord::CInputKeybord()
{
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CInputKeybord::~CInputKeybord()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CInputKeybord::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevise, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevise->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevise->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevise->Acquire();

	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CInputKeybord::Uninit()
{
	CInput::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CInputKeybord::Update()
{
	BYTE aKeyState[256];		//�L�[�{�[�h�̓��͏��

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevise->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int i = 0; i < 256; i++)
		{
			m_aKeyStateTrigger[i] = ~m_aKeyState[i] & aKeyState[i];	// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateRelease[i] = m_aKeyState[i] & ~aKeyState[i];	// �L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyState[i] = aKeyState[i];							// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevise->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//-----------------------------------------
// �v���X���̎擾
//-----------------------------------------
bool CInputKeybord::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) != 0;
}

//-----------------------------------------
// �g���K�[���̎擾
//-----------------------------------------
bool CInputKeybord::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//-----------------------------------------
// �����[�X���̎擾
//-----------------------------------------
bool CInputKeybord::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) != 0;
}
