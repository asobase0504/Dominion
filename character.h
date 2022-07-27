//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CController;
class CBullet;
class CBlock;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CCharacter : public CObject2D
{
public: // ��`
	static const int LIMIT_BULLET_COUNT;
	static const int RELOAD_TIME;

public: // �񋓌^
	enum TEAM
	{
		TEAM_00,
		TEAM_01
	};

public:
	CCharacter(CObject::TYPE type = CObject::TYPE::CHARACTER);
	~CCharacter() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// ���ߎ҂̐ݒ�
	void SetController(CController* inOperate) { m_controller = inOperate; }
	void SetTeam(const TEAM inTeam);

	static CCharacter* Create(TEAM inTeam);

private: // �v���C�x�[�g�֐�
	void ScreenFromOutTime();

	void Collision();
	void HitWithBlock(CBlock* inBlock);
	void HitWithBullet(CBullet* inBullet);

private:
	CController* m_controller;	// ���߂��o���l
	TEAM m_team;				// �������Ă�`�[��
	D3DXVECTOR3 m_move;			// �ړ���
	int m_remainsBulletCount;	// �c��̒e��
	int m_reloadCount;			// �����[�h�J�E���g
};

#endif // !_CHARACTER_H_
