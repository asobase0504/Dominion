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
#include <vector>

class CBullet : public CObject2D
{
public:
	CBullet(CObject::TYPE type = CObject::TYPE::BULLET);
	~CBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �Z�b�^�[
	void SetTeam(const CCharacter::TEAM inTeam);
	bool SetBlockIndex(const int count, std::vector<int> inIndex);

	CCharacter::TEAM GetTeam() { return m_team; }

	static CBullet* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove , const CCharacter::TEAM inTeam);
private: // �v���C�x�[�g�֐�
	void Collision();
	void SetHitBlock(int x, int y);
	void ScreenFromOutTime();
	void HitWithBullet(CBullet* inBullet);
	bool HitWithBlock(CBlock* inBlock);
private:
	D3DXVECTOR3 m_move;			// �ړ���
	CCharacter::TEAM m_team;	// �����`�[��
	int m_life;					// �̗�
	std::vector<std::vector<int>> m_ofBlockIndex;			// ����Ă�u���b�N�̔ԍ�
	int m_ofBlockCount;			// ����Ă�u���b�N�̌�
};
#endif // !_PLAYER_H_
