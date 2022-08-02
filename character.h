//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"
#include <vector>

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
	static const int LIMIT_BULLET_COUNT;	// �e�̍ő吔
	static const int RELOAD_TIME;			// �����[�h����

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

	// �Z�b�^�[
	void SetController(CController* inOperate) { m_controller = inOperate; }
	void SetTeam(const TEAM inTeam);
	bool SetBlockIndex(const int count,std::vector<int> inIndex);

	static CCharacter* Create(TEAM inTeam);

private: // �v���C�x�[�g�֐�
	void ScreenFromOutTime();

	void Collision();
	bool HitWithBlock(CBlock* inBlock);
	void HitWithBullet(CBullet* inBullet);

private:
	CController* m_controller;	// ���߂��o���l
	TEAM m_team;				// �������Ă�`�[��
	D3DXVECTOR3 m_move;			// �ړ���
	int m_remainsBulletCount;	// �c��̒e��
	int m_reloadCount;			// �����[�h�J�E���g
	std::vector<std::vector<int>> m_ofBlockIndex;			// ����Ă�u���b�N�̔ԍ�
};

#endif // !_CHARACTER_H_
