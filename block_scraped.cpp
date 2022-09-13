//==================================================
//
// 削れていくブロック
// Author : Yuda Kaito
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "block_scraped.h"
#include "application.h"
#include "game.h"
#include "mode.h"
#include "map.h"
// デバッグ
#include <assert.h>

//--------------------------------------------------
// 定義
//--------------------------------------------------
const float CBlockScraped::SUBTRACT_SPEED_RATIO = 0.925f;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBlockScraped::CBlockScraped()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CBlockScraped::~CBlockScraped()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
HRESULT CBlockScraped::Init()
{
	return S_OK;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
void CBlockScraped::Uninit()
{
	for (int i = 0; i < (int)m_block.size(); i++)
	{
		if (m_block[i] == nullptr)
		{
			continue;
		}

		m_block[i] = nullptr;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBlockScraped::Update()
{
	for (int i = 0; i < (int)m_block.size(); i++)
	{
		if (m_block[i] == nullptr)
		{
			continue;
		}

		CObject2D* object2d = m_block[i];
		object2d->Update();
		object2d->SetSize(object2d->GetSize() * SUBTRACT_SPEED_RATIO);

		if (object2d->GetSize().x <= 0.0f || object2d->GetSize().y <= 0.0f)
		{
			object2d->SetIsDeleted(true);
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CBlockScraped::Draw()
{
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
CBlockScraped* CBlockScraped::Create(const D3DXVECTOR3& inPos,CBlock::BLOCK_TYPE type, CBlock::DIRECTION inDirection)
{
	CBlockScraped* blockScraped = new CBlockScraped;

	blockScraped->m_block.push_back(new CObject2D);
	blockScraped->m_block.push_back(new CObject2D);

	if (blockScraped->m_block.size() != 2)
	{
		return nullptr;
	}

	for (int i = 0; i < (int)blockScraped->m_block.size(); i++)
	{
		CObject2D* object2d = blockScraped->m_block[i];
		object2d->Init();
		object2d->SetTexture("Block");
		switch (type)
		{
		case CBlock::BLOCK_TYPE::NONE:
			object2d->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			break;
		case CBlock::BLOCK_TYPE::BLOCK_01:
			object2d->SetColor(CApplication::GetInstance()->GetColor(0));
			break;
		case CBlock::BLOCK_TYPE::BLOCK_02:
			object2d->SetColor(CApplication::GetInstance()->GetColor(1));
			break;
		default:
			break;
		}
	}

	switch (inDirection)
	{
	case CBlock::TOP:
		blockScraped->m_block[0]->SetAnchor(CObject2D::Anchor::DOWNRIGHT);
		blockScraped->m_block[0]->SetPos(D3DXVECTOR3(inPos.x - CMap::GetBlockSize() * 0.5f, inPos.y + CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[0]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.9f * 0.5f, CMap::GetBlockSize() * 0.85f));
		blockScraped->m_block[1]->SetAnchor(CObject2D::Anchor::DOWNLEFT);
		blockScraped->m_block[1]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.9f * 0.5f, CMap::GetBlockSize() * 0.85f));
		blockScraped->m_block[1]->SetPos(D3DXVECTOR3(inPos.x + CMap::GetBlockSize() * 0.5f, inPos.y + CMap::GetBlockSize() * 0.5f, inPos.z));
		break;
	case CBlock::DOWN:
		blockScraped->m_block[0]->SetAnchor(CObject2D::Anchor::TOPLEFT);
		blockScraped->m_block[0]->SetPos(D3DXVECTOR3(inPos.x + CMap::GetBlockSize() * 0.5f, inPos.y - CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[0]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.9f * 0.5f, CMap::GetBlockSize() * 0.85f));
		blockScraped->m_block[1]->SetAnchor(CObject2D::Anchor::TOPRIGHT);
		blockScraped->m_block[1]->SetPos(D3DXVECTOR3(inPos.x - CMap::GetBlockSize() * 0.5f, inPos.y - CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[1]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.9f * 0.5f, CMap::GetBlockSize() * 0.85f));
		break;
	case CBlock::LEFT:
		blockScraped->m_block[0]->SetAnchor(CObject2D::Anchor::TOPRIGHT);
		blockScraped->m_block[0]->SetPos(D3DXVECTOR3(inPos.x - CMap::GetBlockSize() * 0.5f, inPos.y - CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[0]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.85f, CMap::GetBlockSize() * 0.9f * 0.5f));
		blockScraped->m_block[1]->SetAnchor(CObject2D::Anchor::DOWNRIGHT);
		blockScraped->m_block[1]->SetPos(D3DXVECTOR3(inPos.x - CMap::GetBlockSize() * 0.5f, inPos.y + CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[1]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.85f, CMap::GetBlockSize() * 0.9f * 0.5f));
		break;
	case CBlock::RIGHT:
		blockScraped->m_block[0]->SetAnchor(CObject2D::Anchor::TOPLEFT);
		blockScraped->m_block[0]->SetPos(D3DXVECTOR3(inPos.x + CMap::GetBlockSize() * 0.5f, inPos.y - CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[0]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.85f, CMap::GetBlockSize() * 0.9f * 0.5f));
		blockScraped->m_block[1]->SetAnchor(CObject2D::Anchor::DOWNLEFT);
		blockScraped->m_block[1]->SetPos(D3DXVECTOR3(inPos.x + CMap::GetBlockSize() * 0.5f, inPos.y + CMap::GetBlockSize() * 0.5f, inPos.z));
		blockScraped->m_block[1]->SetSize(D3DXVECTOR2(CMap::GetBlockSize() * 0.85f, CMap::GetBlockSize() * 0.9f * 0.5f));
		break;
	default:
		break;
	}

	return blockScraped;
}

void CBlockScraped::SetIsDeleted()
{
	for (int i = 0; i < (int)m_block.size(); i++)
	{
		m_block[i]->SetIsDeleted(true);
	}
}
