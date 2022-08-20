//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>
#include <vector>
#include <map>
#include <string>

//==================================================
// ��`
//==================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		TEXTURE_PLAYER = 0,	// �v���C���[�e�N�X�`��
		TEXTURE_BLOCK,		// �u���b�N
		TEXTURE_BULLET,		// �c�e
		TEXTERE_BG,			// �w�i
		TEXTURE_MAX,
		TEXTURE_NONE,	// �g�p���Ȃ�
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();					// �S�Ă̓ǂݍ���
	void UnloadAll();				// �S�Ă̔j��
	void LoadInVector(std::vector<std::string> inTexture);	// �w��̓ǂݍ���
	void Unload(TEXTURE inTexture);	// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(std::string inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	std::map<std::string, LPDIRECT3DTEXTURE9, std::less<>> m_texture;	// �e�N�X�`���̏��
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
