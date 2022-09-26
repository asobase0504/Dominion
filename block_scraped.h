//==================================================
// 
// ���Ă����u���b�N
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
// �u���b�N�N���X
//--------------------------------------------------
class CBlockScraped
{
public:
	static const float SUBTRACT_SPEED_RATIO;
public:
	CBlockScraped();
	~CBlockScraped();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CBlockScraped* Create(const D3DXVECTOR3& inPos,CBlock::BLOCK_TYPE type, CBlock::DIRECTION inDirection, float inSize);

	void SetIsDeleted();
private:
	std::vector<CObject2D*> m_block;
};
#endif // !_BLOCK_H_
