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
	BLOCK_SIZE = (float)CApplication::GetInstance()->SCREEN_HEIGHT / m_blockIdx.size();

	m_block.resize(m_blockIdx.size());
	for (int y = 0; y < (int)m_block.size(); y++)
	{
		float halfY = m_blockIdx.size() * 0.5f;
		for (int x = 0; x < (int)m_blockIdx[y].size(); x++)
		{
			float halfX = m_blockIdx[y].size() * 0.5f;

			m_block[y].push_back(CBlock::Create(m_blockIdx[y][x],this));

			D3DXVECTOR3 pos = {};
			pos.x = (CApplication::GetInstance()->CENTER_X - BLOCK_SIZE * halfX + BLOCK_SIZE * 0.5f) + (x * BLOCK_SIZE);
			pos.y = (CApplication::GetInstance()->CENTER_Y - BLOCK_SIZE * halfY + BLOCK_SIZE * 0.5f) + (y * BLOCK_SIZE);
			pos.z = 0.0f;
			m_block[y][x]->SetPosPlan(pos);
			m_block[y][x]->SetSizePlan(D3DXVECTOR2(BLOCK_SIZE, BLOCK_SIZE));
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
// ���
//--------------------------------------------------
void CMap::Delete()
{
	for (std::vector<CBlock*> block_Y : m_block)
	{
		for (CBlock* block_X : block_Y)
		{
			if (block_X != nullptr)
			{
				block_X->ColorAdditionDelete();
				block_X->ScrapedDelete();
				block_X->SetIsDeleted(true);
				block_X = nullptr;
			}
		}
	}
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
