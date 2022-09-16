//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _AI_OPERATE_H_
#define _AI_OPERATE_H_

#include "controller.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class ASTAR;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CAIController : public CController
{
public:
	CAIController();
	~CAIController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;
private: // �v���C�x�[�g�֐�
	// �ړ����̏�������
	D3DXVECTOR3 MoveToAvoid();
	D3DXVECTOR3 MoveToChase();

	// �e�����Ƃ��̏�������
	CController::SHOT_TYPE ShootToSpreadWay();
	CController::SHOT_TYPE ShootToAttack();
	CController::SHOT_TYPE ShootToOffsetBullet();
private:
	bool isBulletShot;
	ASTAR* m_aStar;
	std::vector<POINT> m_path;
	int m_cellIndex;
	int m_isCellMove;
};

#endif // !_AI_OPERATE_H_
