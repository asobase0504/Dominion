//=========================================
// 
// �C���v�b�g����
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "input.h"

//=========================================
// �ÓI�����o�[�ϐ��̏�����
//=========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//-----------------------------------------
// ������
//-----------------------------------------
CInput::CInput() : 
	m_pDevise(nullptr)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CInput::~CInput()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CInput::Uninit()
{
	// DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
