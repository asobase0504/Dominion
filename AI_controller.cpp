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
	m_isCellMove(false),
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
	std::vector<std::vector<CBlock*>> map = modeGame->GetStage()->GetMap()->GetBlockAll();
	m_aStar->Init(map, m_toOrder->GetTeam());
	ASTAR_PARAM status;
	status.ptStartPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptStartPos.y = m_toOrder->GetCenterBlock()[1];
	status.ptGoalPos.x = m_toOrder->GetCenterBlock()[0] - 10;
	status.ptGoalPos.y = m_toOrder->GetCenterBlock()[1] - 3;
	status.ptCurrentPos.x = m_toOrder->GetCenterBlock()[0];
	status.ptCurrentPos.y = m_toOrder->GetCenterBlock()[1];

	if (SUCCEEDED(m_aStar->CalcPath(&status)))
	{
	}
	else
	{
		int distX = m_toOrder->GetCenterBlock()[0] - status.ptStartPos.x;
		int distY = m_toOrder->GetCenterBlock()[1] - status.ptStartPos.y;

		if (distX == 0 && distY < 0)
		{

		}
		if (distX == 0 && distY > 0)
		{

		}
		if (distY == 0 && distX < 0)
		{

		}
		if (distY == 0 && distX > 0)
		{

		}
	}
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

			for (int i = 0; i < m_toOrder->GetOfBlock().size(); i++)
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
		return D3DXVECTOR3(-0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 move;
	move.x = m_path[m_cellIndex - 1].x - m_path[m_cellIndex].x;
	move.y = m_path[m_cellIndex - 1].y - m_path[m_cellIndex].y;
	move.z = 0.0f;

	return move;
}

//-----------------------------------------
// 弾を発射する処理
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
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
D3DXVECTOR3 CAIController::MoveToChase()
{
	return D3DXVECTOR3();
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
	return NONE_SHOT;
}
