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
#include <stdio.h>
#include <assert.h>

//const int CMap::MAX_X_BLOCK = 40;	// X軸の最大数
//const int CMap::MAX_Y_BLOCK = 17;	// Y軸の最大数
const float CMap::BLOCK_SIZE = 30.0f;	// ブロックのサイズ
const char* CMap::FILE_NAME = "data/map_03.txt";
const float CMap::X_CENTER = CApplication::GetInstance()->SCREEN_WIDTH * 0.5f - (MAX_X_BLOCK * 0.5f * BLOCK_SIZE);	// マップを中央に配置するため(X軸)
const float CMap::Y_CENTER = CApplication::GetInstance()->SCREEN_HEIGHT * 0.5f - (MAX_Y_BLOCK * 0.5f * BLOCK_SIZE);	// マップを中央に配置するため(Y軸)

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
	Load();
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CMap::Uninit()
{
	for (CBlock* block : m_block)
	{
		block->Uninit();
		block = nullptr;
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
	for (CBlock* block : m_block)
	{
		block->Update();
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
// 読込み
//--------------------------------------------------
void CMap::Load()
{
	FILE *pFile = fopen(FILE_NAME, "r");		//ファイルポインタを宣言

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (int y = 0; y < MAX_Y_BLOCK; y++)
		{
			for (int x = 0; x < MAX_X_BLOCK; x++)
			{
				//一行読み込み
				fscanf(pFile, "%d", &m_blockIdx[x][y]);
			}
		}
	}
	else
	{//ファイルが開かない場合
		MessageBox(NULL, TEXT("ファイルが開きませんでした"), TEXT("FILE"), MB_ICONHAND);
		assert(false);
	}
	//ファイルを閉じる
	fclose(pFile);
}

//--------------------------------------------------
// ブロック情報の取得
//--------------------------------------------------
CBlock* CMap::GetBlock()
{
	return m_block[0];
}
