//=========================================
// 
// ���͏����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _INPUT_XINPUT_H_
#define _INPUT_XINPUT_H_

#define DIRECTINPUT_VERSION 0x0800
#include "xinput.h"						// �W���C�p�b�h�����ɕK�v
#pragma comment(lib,"xinput.lib")		//���͏����ɕK�v�i�W���C�p�b�g�j

#include <d3dx9.h>

class CXInput
{
public:
	//�L�[���(�W���C�p�b�h)
	enum JOYKEY
	{
		JOYKEY_CROSS_UP = 0,		//�\���{�^����
		JOYKEY_CROSS_DOWN,			//�\���{�^����
		JOYKEY_CROSS_LEFT,			//�\���{�^����
		JOYKEY_CROSS_RIGHT,			//�\���{�^���E
		JOYKEY_START,				//�X�^�[�g�{�^��
		JOYKEY_BACK,				//�o�b�N�{�^��
		JOYKEY_LEFT_THUMB,			//���X�e�B�b�N������
		JOYKEY_RIGHT_THUMB,			//�E�X�e�B�b�N������
		JOYKEY_LEFT_SHOULDER,		//L�P�{�^��
		JOYKEY_RIGHT_SHOULDER,		//R�P�{�^��
		JOYKEY_LEFT_TRIGGER,		//L�Q�g���K�[
		JOYKEY_RIGHT_TRIGGER,		//R�Q�g���K�[
		JOYKEY_A,					//A�{�^��
		JOYKEY_B,					//B�{�^��
		JOYKEY_X,					//X�{�^��
		JOYKEY_Y,					//Y�{�^��
		JOYKEY_STICK_UP = 0,		//��	(8�����X�e�B�b�N)
		JOYKEY_STICK_UP_RIGHT,		//�E��	(8�����X�e�B�b�N)
		JOYKEY_STICK_RIGHT,			//�E	(8�����X�e�B�b�N)
		JOYKEY_STICK_DOWN_RIGHT,	//�E��	(8�����X�e�B�b�N)
		JOYKEY_STICK_DOWN,			//��	(8�����X�e�B�b�N)
		JOYKEY_STICK_DOWN_LEFT,		//����	(8�����X�e�B�b�N)
		JOYKEY_STICK_LEFT,			//��	(8�����X�e�B�b�N)
		JOYKEY_STICK_UP_LEFT,		//����	(8�����X�e�B�b�N)
		JOYKEY_MAX
	};

	//�X�e�B�b�N����(�W���C�p�b�h)
	enum JOYKEY_RIGHT_LEFT
	{
		JOYKEY_RIGHT_STICK = 0,
		JOYKEY_LEFT_STICK,
		JOYKEY_RIGHT_LEFT_MAX
	};

	//�ړ��n�̃L�[�܂Ƃ�
	enum MOVE_KEY
	{
		MOVE_KEY_UP = 0,		//��L�[
		MOVE_KEY_DOWN,			//���L�[
		MOVE_KEY_RIGHT,			//�E�L�[
		MOVE_KEY_LEFT,			//���L�[
		MOVE_KEY_MAX
	};

	//�L�[�^�C�v
	enum FUNCTION_KEY
	{
		FUNCTION_KEY_DESISION = 0,		//����L�[
		FUNCTION_KEY_PAUSE,				//�|�[�Y�L�[
		FUNCTION_KEY_MAX
	};

	//���C�g�̐F�I���L�[
	enum LIGHT_KEY
	{
		LIGHT_KEY_RED = 0,			//�ԃL�[
		LIGHT_KEY_GREEN,			//�΃L�[
		LIGHT_KEY_BLUE,				//�L�[
		LIGHT_KEY_YELLOW,			//���L�[
		LIGHT_KEY_MAX
	};

public:
	CXInput();
	virtual ~CXInput();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	bool GetJoypadPress(JOYKEY Key);										//�v���X����
	bool GetJoypadTrigger(JOYKEY Key);										//�g���K�[����
	D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft);				//�X�e�B�b�N����
	int GetJoypadTriggerPedal(JOYKEY Key);									//�g���K�[�y�_������
	bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key);		//�X�e�B�b�N�v���X�W����
	bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY Key);	//�X�e�B�b�N�g���K�[�W����			

protected:
	XINPUT_STATE m_JoyKeyState;				//�W���C�p�b�g�̃v���X���
	XINPUT_STATE m_JoyKeyStateTrigger;		//�W���C�p�b�g�̃g���K�[���
	D3DXVECTOR3 m_JoyStickPos;				//�W���C�X�e�B�b�N�̌X��
	JOYKEY m_OldJoyKeyStick[JOYKEY_RIGHT_LEFT_MAX];		//�O��̃X�e�B�b�N�̈ʒu
	FUNCTION_KEY m_OldFunctionKey;						//�O��̋@�\�L�[�̏��
	LIGHT_KEY m_OldLightKey = LIGHT_KEY_MAX;			//�O��̃��C�g�̃L�[�̏��
	MOVE_KEY m_OldMoveKey;

	int m_nKeyCnt;
};

#endif // !_INPUT_XINPUT_H_
