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
#include "main.h"
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
private:
	static const int MAX_X_BLOCK = 40;	// X軸の最大数
	static const int MAX_Y_BLOCK = 17;	// Y軸の最大数
	static const float BLOCK_SIZE;	// ブロックのサイズ
	static const char* FILE_NAME;	// マップファイル名
	static const float X_CENTER;	// マップを中央に配置するため(X軸)
	static const float Y_CENTER;	// マップを中央に配置するため(Y軸)
public:
	CMap();
	~CMap();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Set();
	void Load();
	CBlock* GetBlock();
private:
	std::vector<CBlock*> m_block;
	CBlock::TYPE m_blockIdx[MAX_X_BLOCK][MAX_Y_BLOCK];
};

#endif // !_MAP_H_
