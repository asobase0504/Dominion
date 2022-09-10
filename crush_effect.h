//=========================================
// 
// 砕けるエフェクトの処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _CRUSH_EFFECT_H_
#define _CRUSH_EFFECT_H_

#include "object2d.h"

class CCrushEffect : public CObject2D
{
public:
	CCrushEffect(CObject::TYPE type = CObject::TYPE::NONE);
	~CCrushEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCrushEffect* Create(const D3DXVECTOR3& inPos);

private: // プライベート関数
private:
	int m_life;			// 体力
	D3DXVECTOR3 m_move;	// 移動量
};
#endif // !_PLAYER_H_
