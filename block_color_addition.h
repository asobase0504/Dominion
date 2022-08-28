//==================================================
// 
// 削れていくブロック
// Author : Yuda Kaito
// 
//==================================================
#ifndef _BLOCK_COLOR_ADDITION_H_
#define _BLOCK_COLOR_ADDITION_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include "object2d.h"
#include <vector>
#include "block.h"

//--------------------------------------------------
// ブロッククラス
//--------------------------------------------------
class CBlockColorAddition : public CObject2D
{
private:
	static const int PRIORITY_BELONG;	// このクラスのプライオリティ。
	static const float SUBTRACT_SPEED;	// 減算速度
	static const float BIGIN_ALPHA;		// 最初のα値
public:
	CBlockColorAddition(CObject::TYPE type = CObject::TYPE::NONE);
	~CBlockColorAddition() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBlockColorAddition* Create(const D3DXVECTOR3& inPos, CBlock* inParent);
private:
	CBlock* m_parent;
};
#endif // !_BLOCK_H_
