//==================================================
// 
// ���Ă����u���b�N
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
// �O���錾
//--------------------------------------------------
class CMap;

//--------------------------------------------------
// �u���b�N�N���X
//--------------------------------------------------
class CBlockColorAddition : public CObject2D
{
private:
	static const int PRIORITY_BELONG;	// ���̃N���X�̃v���C�I���e�B�B
	static const float SUBTRACT_SPEED;	// ���Z���x
	static const float BIGIN_ALPHA;		// �ŏ��̃��l
public:
	CBlockColorAddition(CObject::TYPE type = CObject::TYPE::NONE);
	~CBlockColorAddition() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBlockColorAddition* Create(const D3DXVECTOR3& inPos, CBlock* inParent, CMap* inMap);
private:
	CBlock* m_parent;
};
#endif // !_BLOCK_H_
