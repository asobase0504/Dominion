//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _AI_OPERATE_H_
#define _AI_OPERATE_H_

#include "controller.h"
#include "A-Star.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class ASTAR;
class CBullet;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CAIController : public CController
{
public:
	enum AttackWorkaroundMethod
	{
		DODGE,		// ���
		OFFSETTING,	// ����
	};
public:
	CAIController();
	~CAIController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;
private: // �v���C�x�[�g�֐�

	// Setter
	void SetMovePath(const std::vector<POINT>& inPath);

	//A*�̏�����֐�
	ASTAR_PARAM SetAStarParam(POINT inGoal);
	ASTAR_PARAM SetAStarParam(int inX,int inY);

	// �󋵂�c������֐�
	void FindClosestEnemy();
	bool IsBulletHitPos();
	bool ExistsAvoidableSpace();
	bool IsPathCutting();

	// �ړ����̏�������
	D3DXVECTOR3 MoveToAvoid();
	void MoveToChase();

	// �e�����Ƃ��̏�������
	CController::SHOT_TYPE ShootToSpreadWay();
	CController::SHOT_TYPE ShootToAttack();
	CController::SHOT_TYPE ShootToOffsetBullet();
private:
	bool isBulletShot;
	ASTAR* m_aStar;
	SHOT_TYPE m_shotType;
	std::vector<POINT> m_path;
	CCharacter* m_enemy;
	CBullet* m_hitBullet;	// ������\��̃u���b�N
	int m_cellIndex;
	int m_attackCoolDownCount;
	bool m_isCellMove;
	bool m_isEndMove;
};

#endif // !_AI_OPERATE_H_
