//==================================================
//
// �}�b�v�̐���
// Author : Yuda Kaito
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "map.h"
#include "main.h"
#include "block.h"
#include "texture.h"
#include <stdio.h>
#include <assert.h>

//const int CMap::MAX_X_BLOCK = 40;	// X���̍ő吔
//const int CMap::MAX_Y_BLOCK = 17;	// Y���̍ő吔
const float CMap::BLOCK_SIZE = 30.0f;	// �u���b�N�̃T�C�Y
const char* CMap::FILE_NAME = "data/map_03.txt";
const float CMap::X_CENTER = SCREEN_WIDTH * 0.5f - (MAX_X_BLOCK * 0.5f * BLOCK_SIZE);	// �}�b�v�𒆉��ɔz�u���邽��(X��)
const float CMap::Y_CENTER = SCREEN_HEIGHT * 0.5f - (MAX_Y_BLOCK * 0.5f * BLOCK_SIZE);	// �}�b�v�𒆉��ɔz�u���邽��(Y��)

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CMap::CMap()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CMap::~CMap()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CMap::Init()
{
	Load();
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CMap::Uninit()
{
	for (CBlock* block : m_block)
	{
		block->Uninit();
		block = nullptr;
	}

	// �u���b�N�̃N���A
	if (!m_block.empty())
	{
		m_block.clear();
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CMap::Update()
{
	for (CBlock* block : m_block)
	{
		block->Update();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CMap::Draw()
{
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void CMap::Set()
{
	int index;
	for (int y = 0; y < MAX_Y_BLOCK; y++)
	{
		for (int x = 0; x < MAX_X_BLOCK; x++)
		{
			index = y * MAX_X_BLOCK + x;

			m_block.push_back(CBlock::Create(m_blockIdx[x][y]));
			m_block[index]->SetPos(D3DXVECTOR3(x * BLOCK_SIZE + X_CENTER, y * BLOCK_SIZE + Y_CENTER, 0.0f));
			m_block[index]->SetSize(D3DXVECTOR2(BLOCK_SIZE, BLOCK_SIZE));

			//m_block[index].SetTexture(GetTexture(TEXTURE_Number_000));
		}
	}
}

//--------------------------------------------------
// �Ǎ���
//--------------------------------------------------
void CMap::Load()
{
	FILE *pFile = fopen(FILE_NAME, "r");		//�t�@�C���|�C���^��錾

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int y = 0; y < MAX_Y_BLOCK; y++)
		{
			for (int x = 0; x < MAX_X_BLOCK; x++)
			{
				//��s�ǂݍ���
				fscanf(pFile, "%d", &m_blockIdx[x][y]);
			}
		}
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		MessageBox(NULL, TEXT("�t�@�C�����J���܂���ł���"), TEXT("FILE"), MB_ICONHAND);
		assert(false);
	}
	//�t�@�C�������
	fclose(pFile);
}

//--------------------------------------------------
// �u���b�N���̎擾
//--------------------------------------------------
CBlock* CMap::GetBlock()
{
	return m_block[0];
}