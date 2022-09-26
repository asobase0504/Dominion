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
class CRemaubsBullet;
class CStage;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CCharacter : public CObject2D
{
public: // ��`
	static const int LIMIT_BULLET_COUNT;	// �e�̍ő吔
	static const int RELOAD_TIME;			// �����[�h����
	static const float MOVE_SPEAD;			// �ړ����x

public: // �񋓌^
	enum TEAM
	{
		TEAM_00,
		TEAM_01
	};

private: // �v���C�x�[�g�֐�
	enum DIRECTION
	{
		LeftTop = 0,	// ����
		CenterTop,		// ��
		RightTop,		// �E��
		LeftCenter,		// ���^��
		CenterCenter,	// �^��
		RightCenter,	// �E�^��
		LeftBottom,		// ����
		CenterBottom,	// ��
		RightBottom		// �E��
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

	// Setter
	void SetController(CController* inOperate);
	void SetCenterBlockIndex(std::vector<int> inIndex) { m_ofBlockIndexCenter = inIndex; }
	void SetTeam(const TEAM inTeam);
	bool SetBlockIndex(const int count,std::vector<int> inIndex);
	void SetOperationState(const bool inState) { m_isOperationState = inState; }
	void SetStage(CStage* inStage);

	// Getter
	std::vector<std::vector<int>> GetOfBlock() { return m_ofBlockIndex; }
	std::vector<int> GetCenterBlock() { return m_ofBlockIndexCenter; }
	TEAM GetTeam() { return m_team; }
	int GetRemainsBullet() { return m_remainsBulletCount; }

	static CCharacter* Create(TEAM inTeam);

	bool HitWithBlock(CBlock* inBlock);

private: // �v���C�x�[�g�֐�
	void BulletReload();		// �e�������[�h���鏈��
	void ScreenFromOutTime();	// ��ʊO�ɍs���ꍇ�̏���

	void Collision();	// �����蔻��

	void HitWithAnotherTeamBlock(CBlock* inBlock, DIRECTION inDirection, std::vector<DIRECTION> inAround);

private:
	CController* m_controller;	// ���߂��o���l
	CStage* m_stage;	// �X�e�[�W
	TEAM m_team;				// �������Ă�`�[��
	D3DXVECTOR3 m_move;			// �ړ���
	std::vector<CRemaubsBullet*> m_remainsBulletDisplay;	// �\����
	int m_remainsBulletCount;								// �c��̒e��
	int m_reloadCount;										// �����[�h�J�E���g
	std::vector<std::vector<int>> m_ofBlockIndex;			// ����Ă�u���b�N�̔ԍ�
	std::vector<int> m_ofBlockIndexCenter;					// ���S������Ă�u���b�N�̔ԍ�
	int m_ofBlockCount;										// ����Ă�u���b�N�̌�
	float m_spead;											// ���x
	bool isCopied;											// �R�s�[�ς݂�
	bool m_isOperationState;
};

#endif // !_CHARACTER_H_
