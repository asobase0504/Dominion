//==================================================
// 
// デバッグ機能(ヘッダーファイル)
// Author : Yuda Kaito
// 
//==================================================
#ifndef _MAP_H_
#define _MAP_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include "block.h"
#include <vector>

//--------------------------------------------------
// 前方宣言
//--------------------------------------------------
class CBlock;

//--------------------------------------------------
// マップクラス
//--------------------------------------------------
class CMap
{
public:
	CMap();
	~CMap();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Set();
	void Load();

	void Delete();

	// Getter
	CBlock* GetBlock(int indexX, int indexY);
	float GetBlockSize() { return BLOCK_SIZE; }
	int GetMaxXBlock() { return MAX_X_BLOCK; }
	int GetMaxYBlock() { return MAX_Y_BLOCK; }

	// Setter
	void SetMap(std::vector<std::vector<CBlock::BLOCK_TYPE>> inBlockIdx);

private:
	float BLOCK_SIZE;		// ブロックのサイズ
	static const float X_CENTER;	// マップを中央に配置するため(X軸)
	static const float Y_CENTER;	// マップを中央に配置するため(Y軸)
	int MAX_X_BLOCK;	// X軸の最大数
	int MAX_Y_BLOCK;	// Y軸の最大数
	std::vector<std::vector<CBlock*>> m_block;
	std::vector<std::vector<CBlock::BLOCK_TYPE>> m_blockIdx;
};

#endif // !_MAP_H_
