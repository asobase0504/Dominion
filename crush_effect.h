//=========================================
// 
// 砕けるエフェクトの処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _CRUSH_EFFECT_H_
#define _CRUSH_EFFECT_H_

#include "object2d.h"
#include "block.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CMap;

//-----------------------------------------------------------------------------
// 砕けるエフェクト
//-----------------------------------------------------------------------------
class CCrushEffect : public CObject2D
{
public:
	CCrushEffect(CObject::TYPE type = CObject::TYPE::NONE);
	~CCrushEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCrushEffect* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3 inMove,const D3DXCOLOR inColor,const CBlock::BLOCK_TYPE inTeam, CMap* inMap);

private: // プライベート関数
private:
	int m_life;			// 体力
	D3DXVECTOR3 m_move;	// 移動量
	CMap* m_map;
	CBlock::BLOCK_TYPE m_team;
};
#endif // !_PLAYER_H_
