//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// Author  : Yuda Kaito
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

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CTexture::CTexture()
{
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
	
	for (int i = 0; i < (int)list["TEXTURE"].size(); ++i)
	{
		Load(list["TEXTURE"].at(i));
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::Load(std::string inKey, std::string inFileName)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();
	LPDIRECT3DTEXTURE9 texture = nullptr;
	std::string fileName = inFileName;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, &fileName.front(), &texture);

	if (!ExistsKey(inKey))
	{
		m_texture.insert(std::make_pair(inKey, texture));
	}
	else
	{
		m_texture[inKey] = texture;
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::Load(std::vector<std::string> inTexture)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();
	LPDIRECT3DTEXTURE9 texture = nullptr;
	std::string fileName = inTexture[1];

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, &fileName.front(), &texture);

	if (!ExistsKey(inTexture[0]))
	{
		m_texture.insert(std::make_pair(inTexture[0], texture));
	}
	else
	{
		m_texture[inTexture[0]] = texture;
	}
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CTexture::UnloadAll(void)
{
	m_texture.clear();
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CTexture::Unload(std::string inKey)
{
	m_texture.erase(inKey);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(std::string inKey)
{
	// Key�����݂��邩�ۂ��B
	if (!ExistsKey(inKey))
	{
		// Key���Ȃ������ꍇ
		return nullptr;
	}

	/* ��Key���������ꍇ�� */

	return m_texture[inKey];
}
