//=========================================
// 
// �L�[�{�[�h���͏����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_DIRECTINPUT_H_
#define _INPUT_DIRECTINPUT_H_

#include "input.h"
#include <d3dx9.h>

class CDirectInput : public CInput
{
public:
	//�L�[���
	enum JOYKEY
	{								//	PS5				�X�C�b�`por				�X�C�b�`�W���C�R���i�������j
		JOYKEY_DIRECT_0_BUTTON = 0,	//���{�^��			//B�{�^��				//A
		JOYKEY_DIRECT_1_BUTTON,		//�~�{�^��			//A�{�^��				//X
		JOYKEY_DIRECT_2_BUTTON,		//�Z�{�^��			//Y�{�^��				//B
		JOYKEY_DIRECT_3_BUTTON,		//���{�^��			//X�{�^��				//Y
		JOYKEY_DIRECT_4_BUTTON,		//L1�{�^��			//L1�{�^��				//SL
		JOYKEY_DIRECT_5_BUTTON,		//R1�{�^��			//R1�{�^��				//SR
		JOYKEY_DIRECT_6_BUTTON,		//L2�{�^��			//L2�{�^��				//
		JOYKEY_DIRECT_7_BUTTON,		//R2�{�^��			//R2�{�^��				//
		JOYKEY_DIRECT_8_BUTTON,		//�N���G�C�g		//-�{�^��				//
		JOYKEY_DIRECT_9_BUTTON,		//�I�v�V����		//+�{�^��				//
		JOYKEY_DIRECT_10_BUTTON,	//L3�{�^��			//L3�{�^��				//
		JOYKEY_DIRECT_11_BUTTON,	//R3�{�^��			//R3�{�^��				//
		JOYKEY_DIRECT_12_BUTTON,	//�z�[���{�^��		//�z�[���{�^��			//
		JOYKEY_DIRECT_13_BUTTON,	//�^�b�`�p�b�h		//�L���v�`���[			//
		JOYKEY_DIRECT_14_BUTTON,	//		//
		JOYKEY_DIRECT_15_BUTTON,	//		//
		JOYKEY_DIRECT_16_BUTTON,	//		//
		JOYKEY_DIRECT_17_BUTTON,	//���X�e�B�b�N		//
		JOYKEY_DIRECT_18_BUTTON,	//�E�X�e�B�b�N		//
		JOYKEY_DIRECT_19_BUTTON,	//�\���L�[�i��j
		JOYKEY_DIRECT_20_BUTTON,	//�\���L�[�i���j
		JOYKEY_DIRECT_21_BUTTON,	//�\���L�[�i���j
		JOYKEY_DIRECT_22_BUTTON,	//�\���L�[�i�E�j
		JOYKEY_DIRECT_MAX
	};

	//�L�[���(�\���L�[&8�����X�e�B�b�N)
	enum JOYKEY_CROSS
	{
		JOYKEY_CROSS_UP = 0,								//��
		JOYKEY_CROSS_UP_RIGHT = 4500,					//�E��
		JOYKEY_CROSS_RIGHT = 9000,						//�E
		JOYKEY_CROSS_DOWN_RIGHT = 13500,					//�E��
		JOYKEY_CROSS_DOWN = 18000,						//��
		JOYKEY_CROSS_DOWN_LEFT = 22500,					//����
		JOYKEY_CROSS_LEFT = 27000,						//��
		JOYKEY_CROSS_UP_LEFT = 31500,					//����
		JOYKEY_CROSS_MAX
	};

	//�X�e�B�b�N����(�W���C�p�b�h)
	enum JOYKEY_RIGHT_LEFT
	{
		JOYKEY_RIGHT_STICK = 0,
		JOYKEY_LEFT_STICK,
		JOYKEY_RIGHT_LEFT_MAX
	};

public:
	CDirectInput();
	~CDirectInput() override;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(JOYKEY nKey);
	bool GetPress(JOYKEY_CROSS nKey);
	bool GetTrigger(JOYKEY nKey);
	bool GetTrigger(JOYKEY_CROSS nKey);
	bool GetRelease(JOYKEY nKey);
	bool GetRelease(JOYKEY_CROSS nKey);
	D3DXVECTOR3 GetStick(JOYKEY_RIGHT_LEFT Key);
	bool GetStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);
	bool GetStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);

private:
	DIJOYSTATE2 m_PressState;				// �W���C�p�b�g�i�v���X�����j
	DIJOYSTATE2 m_TriggerState;				// �W���C�p�b�g�i�g���K�[�����j
	DIJOYSTATE2 m_ReleaseState;				// �W���C�p�b�g�i�����[�X�����j
	DWORD m_OldJoyKeyDirect = 0;			// �O��̏\���L�[�̒l
	JOYKEY_CROSS m_OldJoyKeyStickDirect;	// �O��̃X�e�B�b�N�̈ʒu
	bool m_bJoyKey = false;					// �g���Ă��邩�ǂ���
};

#endif // !_INPUT_DIRECTINPUT_H_