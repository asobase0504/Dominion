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
// 前方宣言
//-----------------------------------------
class ASTAR;
class CBullet;

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CAIController : public CController
{
public:
	enum AttackWorkaroundMethod
	{
		DODGE,		// 回避
		OFFSETTING,	// 反撃
	};
public:
	CAIController();
	~CAIController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;
private: // プライベート関数

	// Setter
	void SetMovePath(const std::vector<POINT>& inPath);

	//A*の情報代入関数
	ASTAR_PARAM SetAStarParam(POINT inGoal);
	ASTAR_PARAM SetAStarParam(int inX,int inY);

	// 状況を把握する関数
	void FindClosestEnemy();
	bool IsBulletHitPos();
	bool ExistsAvoidableSpace();
	bool IsPathCutting();

	// 移動時の条件動作
	D3DXVECTOR3 MoveToAvoid();
	void MoveToChase();

	// 弾を撃つときの条件動作
	CController::SHOT_TYPE ShootToSpreadWay();
	CController::SHOT_TYPE ShootToAttack();
	CController::SHOT_TYPE ShootToOffsetBullet();
private:
	bool isBulletShot;
	ASTAR* m_aStar;
	SHOT_TYPE m_shotType;
	std::vector<POINT> m_path;
	CCharacter* m_enemy;
	CBullet* m_hitBullet;	// 当たる予定のブロック
	int m_cellIndex;
	int m_attackCoolDownCount;
	bool m_isCellMove;
	bool m_isEndMove;
};

#endif // !_AI_OPERATE_H_
