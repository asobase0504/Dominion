//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _AI_OPERATE_H_
#define _AI_OPERATE_H_

#include "controller.h"
#include "A-Star.h"

//-----------------------------------------
// プレイヤークラス
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
private: // プライベート関数
	// 移動時の条件動作
	D3DXVECTOR3 MoveToAvoid();
	D3DXVECTOR3 MoveToChase();

	// 弾を撃つときの条件動作
	CController::SHOT_TYPE ShootToSpreadWay();
	CController::SHOT_TYPE ShootToAttack();
	CController::SHOT_TYPE ShootToOffsetBullet();
private:
	bool isBulletShot;
};

#endif // !_AI_OPERATE_H_
