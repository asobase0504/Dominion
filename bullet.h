//=========================================
// 
// íeÇÃèàóùÉNÉâÉX
// Author YudaKaito
// 
//=========================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object2d.h"

class CBullet : public CObject2D
{
public:
	CBullet();
	~CBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBullet* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove);
private:
	D3DXVECTOR3 m_move;
	int m_life;
};
#endif // !_PLAYER_H_
