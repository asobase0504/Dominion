//=========================================
// 
// �e�̏����N���X
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
	CBullet(CObject::TYPE type = CObject::TYPE::BULLET);
	~CBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTeam(const CCharacter::TEAM inTeam);
	CCharacter::TEAM GetTeam() { return m_team; }

	static CBullet* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove , const CCharacter::TEAM inTeam);
private: // �v���C�x�[�g�֐�
	void Collision();
	void ScreenFromOutTime();
	void HitWithBullet(CBullet* inBullet);
	void HitWithBlock(CBlock* inBlock);
private:
	D3DXVECTOR3 m_move;			// �ړ���
	CCharacter::TEAM m_team;	// �����`�[��
	int m_life;					// �̗�
};
#endif // !_PLAYER_H_
