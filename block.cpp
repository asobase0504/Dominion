//==================================================
//
// 正方形のポリゴンの生成
// Author : Yuda Kaito
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "block.h"
// デバッグ
#include <assert.h>

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type),
	m_type(TYPE::NONE)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CBlock::~CBlock()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
HRESULT CBlock::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_BLOCK);
	return S_OK;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
void CBlock::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBlock::Update()
{
	//CObject2D::Update();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CBlock::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
CBlock* CBlock::Create(TYPE type)
{
	CBlock* block = new CBlock;

	if (block == nullptr)
	{
		return nullptr;
	}

	block->Init();

	switch (type)
	{
	case TYPE::NONE:
		block->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));
		break;
	case TYPE::BLOCK_01:
		block->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case TYPE::BLOCK_02:
		block->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	default:
		break;
	}

	return block;
}

//--------------------------------------------------
// 種別の切り替え
//--------------------------------------------------
void CBlock::ChangeType(TYPE type)
{
	switch (m_type)
	{
	case TYPE::NONE:
		break;
	case TYPE::BLOCK_01:
		m_type = TYPE::BLOCK_02;
		break;
	case TYPE::BLOCK_02:
		m_type = TYPE::BLOCK_01;
		break;
	default:
		MessageBox(NULL, TEXT("想定外の列挙型を検出。"), TEXT("swith文の条件式"), MB_ICONHAND);
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 種別の取得
//--------------------------------------------------
CBlock::TYPE CBlock::GetType()
{
	return m_type;
}
