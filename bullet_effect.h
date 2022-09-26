//=========================================
// 
// 弾の処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _BULLET_EFFECT_H_
#define _BULLET_EFFECT_H_

#include "object2d.h"

class CBulletEffect : public CObject2D
{
public:
	CBulletEffect(CObject::TYPE type = CObject::TYPE::NONE);
	~CBulletEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBulletEffect* Create(const D3DXVECTOR3& inPos, float inSize);

private: // プライベート関数
private:
	int m_life;					// 体力
};
#endif // !_PLAYER_H_
