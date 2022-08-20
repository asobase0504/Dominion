//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include "file.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
const char* CTexture::s_FileName[] =
{// �e�N�X�`���̃p�X
	"data/TEXTURE/player.png",	// �v���C���[
	"data/TEXTURE/Block.png",	// �u���b�N
	"data/TEXTURE/Bullet.png",	// �c�e
	"data/TEXTURE/bg.png",	// �c�e
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CTexture::CTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// �S�Ă̓ǂݍ���
//--------------------------------------------------
void CTexture::LoadAll()
{
	nlohmann::json list = LoadJsonStage(L"data/FILE/texture.json");
	
	for (int i = 0; i < list["TEXTURE"].size(); ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̓ǂݍ��݂�����Ă���
			continue;
		}

		LoadInVector(list["TEXTURE"].at(i));
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::LoadInVector(std::vector<std::string> inTexture)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	std::string fileName = inTexture[1];

	LPDIRECT3DTEXTURE9 texture;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, &fileName.front(), &texture);

	m_texture.insert(std::make_pair(inTexture[0], texture));
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CTexture::UnloadAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CTexture::Unload(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// �e�N�X�`���̉��
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = nullptr;
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(std::string inTexture)
{
	return m_texture[inTexture];
}
