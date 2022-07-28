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
#include "input_xinput.h"

//=========================================
// �ÓI�����o�[�ϐ��̏�����
//=========================================

//-----------------------------------------
// ������
//-----------------------------------------
CInput::CInput() : 
	m_pDevise(nullptr),
	m_pInput(nullptr)
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
	// ���̓f�o�C�X�̕���
	if (m_pDevise != NULL)
	{
		m_pDevise->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevise->Release();
		m_pDevise = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
