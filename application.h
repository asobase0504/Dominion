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
class CTexture;
class CMap;

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

	CRenderer* GetRenderer();
	CObject** GetMyObject();
	CInputKeybord* GetInput();
	CTexture* GetTextureClass();

private: // ���I�����o�[�ϐ�
	CRenderer* renderer;
	CInputKeybord* input;
	CTexture* texture;
	CObject* object;
	CCharacter* character;
	CMap* map;
};

#endif // !_APPLICATION_H_
