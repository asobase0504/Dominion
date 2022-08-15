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
class CInputKeybord;
class CDirectInput;
class CTexture;
class CMode;
class CColor;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CApplication
{
public:
	enum class MODE_TYPE
	{
		TITLE = 0,
		GAME,
		RESULT,
		MAX
	};

public: // �萔
	const int SCREEN_WIDTH;		// �X�N���[���̕�
	const int SCREEN_HEIGHT;	// �X�N���[���̍���
	const float CENTER_X;			// �X�N���[�����S���W��X���W
	const float CENTER_Y;			// �X�N���[�����S���W��Y���W

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

	// Getter
	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CInputKeybord* GetInput() { return input; }
	CDirectInput* GetDirectInput() { return directInput; }
	CTexture* GetTextureClass() { return texture; }
	CMode* GetMode() { return mode; }
	D3DXCOLOR GetColor(int inKey);

	// Setter
	void SetMode(MODE_TYPE inType);

private: // ���I�����o�[�ϐ�
	CMode* mode;
	CRenderer* renderer;
	CInputKeybord* input;
	CDirectInput* directInput;
	CTexture* texture;
	CObject* object;
	CColor* color;
};

#endif // !_APPLICATION_H_
