//==================================================
// 
// 削れていくブロック
// Author : Yuda Kaito
// 
//==================================================
#ifndef _BLOCK_SCRAPED_H_
#define _BLOCK_SCRAPED_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include "object2d.h"
#include <vector>
#include "block.h"

//--------------------------------------------------
// ブロッククラス
//--------------------------------------------------
class CBlockScraped
{
public:

public:
	CBlockScraped();
	~CBlockScraped();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CBlockScraped* Create(const D3DXVECTOR3& inPos,CBlock::BLOCK_TYPE type, CBlock::DIRECTION inDirection);

	bool GetIsDeleted() { return m_isDeleted; }
private:
	std::vector<CObject2D*> m_block;
	bool m_isDeleted;
};
#endif // !_BLOCK_H_
