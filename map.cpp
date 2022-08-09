//==================================================
//
// マップの生成
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

//const int CMap::MAX_X_BLOCK = 40;	// X軸の最大数
//const int CMap::MAX_Y_BLOCK = 17;	// Y軸の最大数
float CMap::BLOCK_SIZE = 34.0f;	// ブロックのサイズ
const char* CMap::FILE_NAME = "data/FILE/MAP/map_03.txt";
const float CMap::X_CENTER = CApplication::GetInstance()->SCREEN_WIDTH * 0.5f ;	// マップを中央に配置するため(X軸)
const float CMap::Y_CENTER = CApplication::GetInstance()->SCREEN_HEIGHT * 0.5f;	// マップを中央に配置するため(Y軸)

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CMap::CMap()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CMap::~CMap()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CMap::Init()
{
	return S_OK;
}

//--------------------------------------------------
// 終了
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

	// ブロックのクリア
	if (!m_block.empty())
	{
		m_block.clear();
	}
}

//--------------------------------------------------
// 更新
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
// 描画
//--------------------------------------------------
void CMap::Draw()
{
}

//--------------------------------------------------
// 設定
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
// 読込み
//--------------------------------------------------
void CMap::Load()
{

}

//--------------------------------------------------
// ブロック情報の取得
//--------------------------------------------------
CBlock* CMap::GetBlock(int indexX, int indexY)
{
	return m_block[indexY][indexX];
}

//--------------------------------------------------
// ブロック番号情報の設定
//--------------------------------------------------
void CMap::SetMap(std::vector<std::vector<CBlock::BLOCK_TYPE>> inBlockIdx)
{
	m_blockIdx = inBlockIdx;
	MAX_Y_BLOCK = (int)m_blockIdx.size();
	MAX_X_BLOCK = (int)m_blockIdx[0].size();
}
