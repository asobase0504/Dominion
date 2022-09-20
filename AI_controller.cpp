//=========================================
// 
// AIからの入力命令クラス
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "AI_controller.h"
#include "object.h"
#include "bullet.h"
#include "A-Star.h"
#include "application.h"
#include "game.h"
#include "stage.h"
#include "map.h"
#include <vector>

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, __FILE__, __LINE__, NULL, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CAIController::CAIController() : 
	isBulletShot(false),
	m_aStar(nullptr),
	m_enemy(nullptr),
	m_isCellMove(false),
	m_isEndMove(true),
	m_shotType(NONE_SHOT)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CAIController::~CAIController()
{
	assert(m_aStar == nullptr);
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CAIController::Init()
{
	m_aStar = new ASTAR;

	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	m_aStar->Init(modeGame->GetStage()->GetMap()->GetBlockAll(), m_toOrder->GetTeam());
	m_path = m_aStar->GetPath();

	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CAIController::Uninit()
{
	if (m_aStar != nullptr)
	{
		m_aStar->Uninit();
		delete m_aStar;
		m_aStar = nullptr;
	}
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CAIController::Update()
{
	m_shotType = NONE_SHOT;
	FindClosestEnemy();

	if (m_enemy == nullptr)
	{
		return;
	}

	if (m_isEndMove)
	{
		m_isEndMove = false;
		MoveToChase();
	}

	if (m_shotType == NONE_SHOT)
	{
		m_shotType = ShootToOffsetBullet();
	}
}

//-----------------------------------------
// 移動
//-----------------------------------------
D3DXVECTOR3 CAIController::Move()
{
	if (m_isCellMove)
	{
		if (m_cellIndex != 0)
		{
			bool isCenterHit = (m_path[m_cellIndex - 1].x == m_toOrder->GetCenterBlock()[0]) && (m_path[m_cellIndex - 1].y == m_toOrder->GetCenterBlock()[1]);

			int ofBlock = 0;

			for (int i = 0; i < (int)m_toOrder->GetOfBlock().size(); i++)
			{
				if (!m_toOrder->GetOfBlock()[i].empty())
				{
					ofBlock++;
				}
			}

			if (isCenterHit && ofBlock <= 1)
			{
				m_isCellMove = false;
			}
		}
	}

	if (!m_isCellMove)
	{
		m_cellIndex = 0;

		DEBUG_PRINT("Order : x = %d,y = %d\n", m_toOrder->GetCenterBlock()[0], m_toOrder->GetCenterBlock()[1]);	// 移動先をデバッグ表示
		for (int i = 0; i < (int)m_path.size(); i++)
		{
			bool isCenterHit = (m_path[i].x == m_toOrder->GetCenterBlock()[0]) && (m_path[i].y == m_toOrder->GetCenterBlock()[1]);

			DEBUG_PRINT("path  : x = %d,y = %d\n", m_path[i].x, m_path[i].y);	// 検索を掛けたパスをデバッグ表示

			if (!(isCenterHit))
			{
				continue;
			}

			DEBUG_PRINT("success : x = %d,y = %d\n", m_toOrder->GetCenterBlock()[0], m_toOrder->GetCenterBlock()[1]);	// 出力にデバッグ表示
			m_cellIndex = i;
			break;
		}
		m_isCellMove = true;
	}

	if (m_cellIndex == 0)
	{
		m_isCellMove = false;
		m_isEndMove = true;
		return D3DXVECTOR3(-0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 move;
	move.x = (float)(m_path[m_cellIndex - 1].x - m_path[m_cellIndex].x);
	move.y = (float)(m_path[m_cellIndex - 1].y - m_path[m_cellIndex].y);
	move.z = 0.0f;

	return move;
}

//-----------------------------------------
// 弾を発射する処理
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
{
	return m_shotType;
}

ASTAR_PARAM CAIController::SetAStarParam(POINT inGoal)
{
	ASTAR_PARAM status;
	status.ptStartPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptStartPos.y = m_toOrder->GetCenterBlock()[1];
	status.ptCurrentPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptCurrentPos.y = m_toOrder->GetCenterBlock()[1];

	status.ptGoalPos.x = inGoal.x;
	status.ptGoalPos.y = inGoal.y;

	return status;
}

ASTAR_PARAM CAIController::SetAStarParam(int inX, int inY)
{
	ASTAR_PARAM status;
	status.ptStartPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptStartPos.y = m_toOrder->GetCenterBlock()[1];
	status.ptCurrentPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptCurrentPos.y = m_toOrder->GetCenterBlock()[1];

	status.ptGoalPos.x = inX;
	status.ptGoalPos.y = inY;

	return status;
}

//-----------------------------------------------------------------------------
// 一番近い敵を探す
//-----------------------------------------------------------------------------
void CAIController::FindClosestEnemy()
{
	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	std::vector<CCharacter*> character = *modeGame->GetStage()->GetCharacter();
	int dist = 999;

	for (int i = 0; i < (int)character.size(); i++)
	{
		if (m_toOrder->GetTeam() == character[i]->GetTeam())
		{
			continue;
		}

		int distX = m_toOrder->GetCenterBlock()[0] - character[i]->GetCenterBlock()[0];
		int distY = m_toOrder->GetCenterBlock()[1] - character[i]->GetCenterBlock()[1];

		if (dist > distX + distY)
		{
			dist = distX + distY;
			m_enemy = character[i];
		}
	}
}

//-----------------------------------------------------------------------------
// 弾が当たる位置にいる
//-----------------------------------------------------------------------------
bool CAIController::IsBulletHitPos()
{
	return false;
}

//-----------------------------------------------------------------------------
// 避けるためのスペースが存在する
//-----------------------------------------------------------------------------
bool CAIController::ExistsAvoidableSpace()
{
	return false;
}

//-----------------------------------------------------------------------------
// 避けるために移動する
//-----------------------------------------------------------------------------
D3DXVECTOR3 CAIController::MoveToAvoid()
{
	return D3DXVECTOR3();
}

//-----------------------------------------------------------------------------
// 追うために移動する
//-----------------------------------------------------------------------------
void CAIController::MoveToChase()
{
	ASTAR_PARAM status = SetAStarParam(m_toOrder->GetCenterBlock()[0], m_enemy->GetCenterBlock()[1]);

	std::vector<POINT> routePlanX;
	bool isConnectX;
	if (SUCCEEDED(m_aStar->CalcPath(&status)))
	{
		isConnectX = true;
		routePlanX = m_aStar->GetPath();
	}
	else
	{
		isConnectX = false;
	}

	status = SetAStarParam(m_enemy->GetCenterBlock()[0], m_toOrder->GetCenterBlock()[1]);

	std::vector<POINT> routePlanY;
	bool isConnectY;
	if (SUCCEEDED(m_aStar->CalcPath(&status)))
	{
		isConnectY = true;
		routePlanY = m_aStar->GetPath();
	}
	else
	{
		isConnectY = false;

	}

	if (!isConnectX && !isConnectY)
	{
		/* ↓パスが作れなかった場合↓ */
		m_shotType = LEFT_SHOT;
		return;
	}

	/* ↓パスが作れた場合↓ */

	if (isConnectX && !isConnectY)
	{
		m_path = routePlanX;
	}
	if (!isConnectX && isConnectY)
	{
		m_path = routePlanY;
	}
	if (isConnectX && isConnectY)
	{
		if (routePlanX.size() <= routePlanY.size())
		{
			m_path = routePlanX;
		}
		else
		{
			m_path = routePlanY;
		}
	}
}

//-----------------------------------------------------------------------------
// 道を広げるために弾を撃つ
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToSpreadWay()
{
	return NONE_SHOT;
}

//-----------------------------------------------------------------------------
// キャラクターを攻撃するために弾を撃つ
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToAttack()
{
	return NONE_SHOT;
}

//-----------------------------------------------------------------------------
// 弾を相殺するために弾を撃つ
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToOffsetBullet()
{
	static int count = 0;	// 弾発射の間隔。
	CCharacter* charcter = m_toOrder;
	std::vector<std::vector<int>> ofBlockCharcter = charcter->GetOfBlock();

	// オブジェクトを全てチェックする
	for (int cnt = 0; cnt < CObject::GetPrioritySize(); cnt++)
	{
		for (auto it = CObject::GetMyObject(cnt)->begin(); it != CObject::GetMyObject(cnt)->end(); it++)
		{
			if ((*it)->GetIsDeleted())
			{
				continue;
			}

			/* ↓オブジェクトが死ぬ予定がない場合↓ */

			if (!((*it)->CObject::GetType() == CObject::TYPE::BULLET))
			{
				continue;
			}

			/* ↓オブジェクトが弾の場合↓ */

			CBullet* bullet = (CBullet*)(*it);

			if ((int)bullet->GetTeam() == (int)charcter->GetTeam())
			{
				continue;
			}

			if (isBulletShot)
			{
				count++;
				if (count >= 15)
				{
					isBulletShot = false;
					count = 0;
				}
				continue;
			}

			// 対象所属のブロック取得
			std::vector<std::vector<int>> ofBlockTarget = bullet->GetOfBlock();

			// 自身が所属しているブロックを全部チェックする
			for (int i = 0; i < 4; i++)
			{
				if (ofBlockCharcter[i].empty())
				{
					continue;
				}

				/* ↓所属ブロックがあった場合↓ */

				// 対象が所属しているブロックを全部チェックする
				for (int j = 0; j < 4; j++)
				{
					if (ofBlockTarget[j].empty())
					{
						continue;
					}

					/* ↓所属ブロックがあった場合↓ */

					bool isXAxisMatched = ofBlockCharcter[i][0] == ofBlockTarget[j][0];	// X軸の一致
					bool isYAxisMatched = ofBlockCharcter[i][1] == ofBlockTarget[j][1];	// Y軸の一致

					if (isXAxisMatched)
					{
						if ((ofBlockCharcter[i][1] - 5 < ofBlockTarget[j][1]) && (ofBlockCharcter[i][1] > ofBlockTarget[j][1]))
						{
							isBulletShot = true;
							return UP_SHOT;
						}
						else if ((ofBlockCharcter[i][1] + 5 > ofBlockTarget[j][1]) && (ofBlockCharcter[i][1] < ofBlockTarget[j][1]))
						{
							isBulletShot = true;
							return DOWN_SHOT;
						}
					}
					else if (isYAxisMatched)
					{
						if ((ofBlockCharcter[i][0] - 5 < ofBlockTarget[j][0]) && (ofBlockCharcter[i][0] > ofBlockTarget[j][0]))
						{
							isBulletShot = true;
							return LEFT_SHOT;
						}
						else if ((ofBlockCharcter[i][0] + 5 > ofBlockTarget[j][0]) && (ofBlockCharcter[i][0] < ofBlockTarget[j][0]))
						{
							isBulletShot = true;
							return RIGHT_SHOT;
						}
					}
				}
			}
		}
	}
	return NONE_SHOT;
}
