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

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CMap;

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
class CBullet : public CObject2D
{
public:
	CBullet(CObject::TYPE type = CObject::TYPE::BULLET);
	~CBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetTeam(const CCharacter::TEAM inTeam);
	bool SetBlockIndex(const int count, std::vector<int> inIndex);

	// Getter
	CCharacter::TEAM GetTeam() { return m_team; }
	std::vector<std::vector<int>> GetOfBlock() { return m_ofBlockIndex; }

	static CBullet* Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove , const CCharacter::TEAM inTeam,CMap* inMap);

	bool HitWithBlock(CBlock* inBlock);
private: // �v���C�x�[�g�֐�
	void Collision();
	void ScreenFromOutTime();
	void HitWithBullet(CBullet* inBullet);
private:
	D3DXVECTOR3 m_move;			// �ړ���
	CMap* m_map;				// �}�b�v���
	CCharacter::TEAM m_team;	// �����`�[��
	int m_life;					// �̗�
	std::vector<std::vector<int>> m_ofBlockIndex;			// ����Ă�u���b�N�̔ԍ�
	int m_ofBlockCount;			// ����Ă�u���b�N�̌�
};
#endif // !_PLAYER_H_
