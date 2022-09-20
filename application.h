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
class CDirectInput;
class CTexture;
class CMode;
class CThemeColor;
class CInput;
class CSound;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CApplication
{
public:
	enum class MODE_TYPE
	{
		TITLE = 0,
		TUTORIAL,
		CUSTUM,
		GAME,
		RESULT,
		MAX
	};

public: // �萔
	const int SCREEN_WIDTH;		// �X�N���[���̕�
	const int SCREEN_HEIGHT;	// �X�N���[���̍���
	const float CENTER_X;		// �X�N���[�����S���W��X���W
	const float CENTER_Y;		// �X�N���[�����S���W��Y���W

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
	CTexture* GetTextureClass() { return texture; }
	CMode* GetMode() { return mode; }
	CSound* GetSound() { return sound; }
	D3DXCOLOR GetColor(int inKey);
	int GetColorSize();
	D3DXVECTOR2 GetScreenSize() { return D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT); }
	D3DXVECTOR2 GetScreenCenter() { return D3DXVECTOR2(CENTER_X, CENTER_Y); }

	// Setter
	void SetMode(MODE_TYPE inType);
	void SetThemeColor(int idx);

private: // ���I�����o�[�ϐ�
	CMode* mode;
	CInput* input;
	CRenderer* renderer;
	CTexture* texture;
	CObject* object;
	CThemeColor* color;
	CSound* sound;
};

#endif // !_APPLICATION_H_
