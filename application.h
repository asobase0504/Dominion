//=========================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>
#include <vector>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CRenderer;
class CObject;
class CCharacter;
class CInputKeybord;
class CDirectInput;
class CTexture;
class CMap;
class CGame;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CApplication
{
public: // �萔
	const int SCREEN_WIDTH = 1280;	// �X�N���[���̕�
	const int SCREEN_HEIGHT = 720;	// �X�N���[���̍���

public: // �ÓI�����o�[�ϐ�
	static CApplication* application;
public: // �ÓI�֐�
	static CApplication* GetInstance();

private: // �V���O���g��
	CApplication();
public:	// �����o�[�֐�
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CInputKeybord* GetInput() { return input; }
	CDirectInput* GetDirectInput() { return directInput; }
	CTexture* GetTextureClass() { return texture; }
	CGame* GetGame() { return game; }

private: // ���I�����o�[�ϐ�
	CGame* game;
	CRenderer* renderer;
	CInputKeybord* input;
	CDirectInput* directInput;
	CTexture* texture;
	CObject* object;
};

#endif // !_APPLICATION_H_
