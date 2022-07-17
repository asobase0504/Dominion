//=========================================
// 
// 弾の処理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object2d.h"
#include "character.h"

class CBullet : public CObject2D
{
public:
	CBullet();
	~CBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTeam(const CCharacter::TEAM inTeam);

	static CBullet* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove , const CCharacter::TEAM inTeam);
private: // プライベート関数
	void ScreenFromOutTime();
	void HitWithBullet();
private:
	D3DXVECTOR3 m_move;			// 移動量
	CCharacter::TEAM m_team;	// 所属チーム
	int m_life;					// 体力
};
#endif // !_PLAYER_H_
