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
#include "block.h"
#include "application.h"
#include "texture.h"
#include <assert.h>
#include "file.h"

//const int CMap::MAX_X_BLOCK = 40;	// X���̍ő吔
//const int CMap::MAX_Y_BLOCK = 17;	// Y���̍ő吔
float CMap::BLOCK_SIZE = 34.0f;	// �u���b�N�̃T�C�Y
const char* CMap::FILE_NAME = "data/FILE/MAP/map_03.txt";
const float CMap::X_CENTER = CApplication::GetInstance()->SCREEN_WIDTH * 0.5f ;	// �}�b�v�𒆉��ɔz�u���邽��(X��)
const float CMap::Y_CENTER = CApplication::GetInstance()->SCREEN_HEIGHT * 0.5f;	// �}�b�v�𒆉��ɔz�u���邽��(Y��)

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
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CMap::Uninit()
{
	for (std::vector<CBlock*> block_Y : m_block)
	{
		for (CBlock* block_X : block_Y)
		{
			block_X = nullptr;
		}
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
	for (std::vector<CBlock*> block_Y : m_block)
	{
		for (CBlock* block_X : block_Y)
		{
			block_X->Update();
		}
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
	BLOCK_SIZE = (float)CApplication::GetInstance()->SCREEN_HEIGHT / m_blockIdx.size();
	for (int y = 0; y < m_blockIdx.size(); y++)
	{
		std::vector<CBlock*> a;
		m_block.push_back(a);
		for (int x = 0; x < m_blockIdx[y].size(); x++)
		{
			index = y * MAX_X_BLOCK + x;

			m_block[y].push_back(CBlock::Create(m_blockIdx[y][x]));
			m_block[y][x]->SetPos(D3DXVECTOR3(x * BLOCK_SIZE + (BLOCK_SIZE * 0.5f), y * BLOCK_SIZE + (BLOCK_SIZE * 0.5f), 0.0f));
			m_block[y][x]->SetSize(D3DXVECTOR2(BLOCK_SIZE, BLOCK_SIZE));
		}
	}
}

//--------------------------------------------------
// �Ǎ���
//--------------------------------------------------
void CMap::Load()
{

}

//--------------------------------------------------
// �u���b�N���̎擾
//--------------------------------------------------
CBlock* CMap::GetBlock(int indexX, int indexY)
{
	return m_block[indexY][indexX];
}

//--------------------------------------------------
// �u���b�N�ԍ����̐ݒ�
//--------------------------------------------------
void CMap::SetMap(std::vector<std::vector<CBlock::BLOCK_TYPE>> inBlockIdx)
{
	m_blockIdx = inBlockIdx;
	MAX_Y_BLOCK = (int)m_blockIdx.size();
	MAX_X_BLOCK = (int)m_blockIdx[0].size();
}
