//=========================================
// 
// AI����̓��͖��߃N���X
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

#include "utility.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CAIController::CAIController() : 
	isBulletShot(false),
	m_aStar(nullptr),
	m_enemy(nullptr),
	m_hitBullet(nullptr),
	m_isCellMove(false),
	m_isEndMove(true),
	m_shotType(NONE_SHOT),
	m_attackCoolDownCount(0)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CAIController::~CAIController()
{
	assert(m_aStar == nullptr);
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CAIController::Init()
{
	m_aStar = new ASTAR;

	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	m_aStar->Init(modeGame->GetStage()->GetMap()->GetBlockAll(), m_toOrder->GetTeam());
	SetMovePath(m_aStar->GetPath());

	return S_OK;
}

//-----------------------------------------
// �I��
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
// �X�V
//-----------------------------------------
void CAIController::Update()
{
	m_shotType = NONE_SHOT;
	FindClosestEnemy();

	if (m_enemy == nullptr)
	{
		return;
	}

	if (m_isEndMove || IsPathCutting())
	{
		m_isEndMove = false;
		MoveToChase();
	}

	bool hitBulletPlan = IsBulletHitPos();
	DEBUG_PRINT("HitBullet : %s\n", hitBulletPlan ? "true" : "false");	// �ړ�����f�o�b�O�\��

	if (hitBulletPlan)
	{
		bool existsShootPlan = m_shotType == NONE_SHOT;	// �e�����\��͂��邩
		bool existsBulletCountAllowance = m_toOrder->GetRemainsBullet() > 3;	// �e���̗]�T�͂��邩
		bool existsAroundSpaceAllowance;	// ����̃X�y�[�X�ɗ]�T�͂��邩

		if (existsShootPlan && existsBulletCountAllowance)
		{
			m_shotType = ShootToOffsetBullet();
		}
	}

	/* ���ˌ������肵�ĂȂ������火 */
	if (m_shotType == NONE_SHOT)
	{
		m_shotType = ShootToAttack();
	}
}

//-----------------------------------------
// �ړ�
//-----------------------------------------
D3DXVECTOR3 CAIController::Move()
{
	if (m_isCellMove)
	{
		if (m_cellIndex < 0)
		{
			bool isCenterHit = (m_path.at(m_cellIndex - 1).x == m_toOrder->GetCenterBlock().at(0)) && (m_path.at(m_cellIndex - 1).y == m_toOrder->GetCenterBlock().at(1));

			int ofBlock = 0;

			for (int i = 0; i < (int)m_toOrder->GetOfBlock().size(); i++)
			{
				if (!m_toOrder->GetOfBlock().at(i).empty())
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

		DEBUG_PRINT("MovingSearch\n");	// �ړ�����f�o�b�O�\��
		DEBUG_PRINT("Order : x = %d,y = %d\n", m_toOrder->GetCenterBlock().at(0), m_toOrder->GetCenterBlock().at(1));	// �ړ�����f�o�b�O�\��
		for (int i = 0; i < (int)m_path.size(); i++)
		{
			bool isCenterHit = (m_path.at(i).x == m_toOrder->GetCenterBlock().at(0)) && (m_path.at(i).y == m_toOrder->GetCenterBlock().at(1));

			DEBUG_PRINT("path  : x = %d,y = %d\n", m_path.at(i).x, m_path.at(i).y);	// �������|�����p�X���f�o�b�O�\��

			if (!(isCenterHit))
			{
				continue;
			}

			DEBUG_PRINT("success : x = %d,y = %d\n", m_toOrder->GetCenterBlock().at(0), m_toOrder->GetCenterBlock().at(1));	// �o�͂Ƀf�o�b�O�\��
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
	move.x = (float)(m_path.at(m_cellIndex - 1).x - m_path.at(m_cellIndex).x);
	move.y = (float)(m_path.at(m_cellIndex - 1).y - m_path.at(m_cellIndex).y);
	move.z = 0.0f;

	return move;
}

//-----------------------------------------
// �e�𔭎˂��鏈��
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
{
	return m_shotType;
}

//-----------------------------------------
// �ړ��p�X��ݒ�
//-----------------------------------------
void CAIController::SetMovePath(const std::vector<POINT>& inPath)
{
	m_cellIndex = 0;
	m_isEndMove = true;
	m_path = inPath;
}

//-----------------------------------------
// A*Param�̏���ݒ肷��
//-----------------------------------------
ASTAR_PARAM CAIController::SetAStarParam(POINT inGoal)
{
	ASTAR_PARAM status;
	status.ptStartPos.x = m_toOrder->GetCenterBlock().at(0);
	status.ptStartPos.y = m_toOrder->GetCenterBlock().at(1);
	status.ptCurrentPos.x = m_toOrder->GetCenterBlock().at(0);
	status.ptCurrentPos.y = m_toOrder->GetCenterBlock().at(1);

	status.ptGoalPos.x = inGoal.x;
	status.ptGoalPos.y = inGoal.y;

	return status;
}

//-----------------------------------------
// A*Param�̏���ݒ肷��
//-----------------------------------------
ASTAR_PARAM CAIController::SetAStarParam(int inX, int inY)
{
	ASTAR_PARAM status;
	status.ptStartPos.x = m_toOrder->GetCenterBlock().at(0);
	status.ptStartPos.y = m_toOrder->GetCenterBlock().at(1);
	status.ptCurrentPos.x = m_toOrder->GetCenterBlock().at(0);
	status.ptCurrentPos.y = m_toOrder->GetCenterBlock().at(1);

	status.ptGoalPos.x = inX;
	status.ptGoalPos.y = inY;

	return status;
}

//-----------------------------------------------------------------------------
// ��ԋ߂��G��T��
//-----------------------------------------------------------------------------
void CAIController::FindClosestEnemy()
{
	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	std::vector<CCharacter*> character = *modeGame->GetStage()->GetCharacter();
	int dist = 999;

	for (int i = 0; i < (int)character.size(); i++)
	{
		if (m_toOrder->GetTeam() == character.at(i)->GetTeam())
		{
			continue;
		}

		int distX = m_toOrder->GetCenterBlock().at(0) - character.at(i)->GetCenterBlock().at(0);
		int distY = m_toOrder->GetCenterBlock().at(1) - character.at(i)->GetCenterBlock().at(1);

		if (dist > distX + distY)
		{
			dist = distX + distY;
			m_enemy = character.at(i);
		}
	}
}

//-----------------------------------------------------------------------------
// �e��������ʒu�ɂ���
//-----------------------------------------------------------------------------
bool CAIController::IsBulletHitPos()
{
	std::vector<std::vector<int>> ofBlockCharcter = m_toOrder->GetOfBlock();

	// �I�u�W�F�N�g��S�ă`�F�b�N����
	for (int cnt = 0; cnt < CObject::GetPrioritySize(); cnt++)
	{
		for (auto it = CObject::GetMyObject(cnt)->begin(); it != CObject::GetMyObject(cnt)->end(); it++)
		{
			if ((*it)->GetIsDeleted())
			{
				continue;
			}

			/* ���I�u�W�F�N�g�����ʗ\�肪�Ȃ��ꍇ�� */

			if (!((*it)->CObject::GetType() == CObject::TYPE::BULLET))
			{
				continue;
			}

			/* ���I�u�W�F�N�g���e�̏ꍇ�� */

			CBullet* bullet = (CBullet*)(*it);

			if ((int)bullet->GetTeam() == (int)m_toOrder->GetTeam())
			{
				continue;
			}

			/* �����g�ƈႤ�`�[���̒e�������ꍇ�� */

			// �Ώۏ����̃u���b�N�擾
			std::vector<std::vector<int>> ofBlockTarget = bullet->GetOfBlock();

			// ���g���������Ă���u���b�N��S���`�F�b�N����
			for (int i = 0; i < (int)ofBlockCharcter.size(); i++)
			{
				if (ofBlockCharcter.at(i).empty())
				{
					continue;
				}

				/* �������u���b�N���������ꍇ�� */

				// �Ώۂ��������Ă���u���b�N��S���`�F�b�N����
				for (int j = 0; j < 4; j++)
				{
					if (ofBlockTarget.at(j).empty())
					{
						continue;
					}

					/* �������u���b�N���������ꍇ�� */

					bool isXAxisMatched = ofBlockCharcter.at(i).at(0) == ofBlockTarget.at(j).at(0);	// X���̈�v
					bool isYAxisMatched = ofBlockCharcter.at(i).at(1) == ofBlockTarget.at(j).at(1);	// Y���̈�v
					if (isXAxisMatched || isYAxisMatched)
					{
						m_hitBullet = bullet;
						return true;
					}
				}
			}
		}
	}
	m_hitBullet = nullptr;
	return false;
}

//-----------------------------------------------------------------------------
// �����邽�߂̃X�y�[�X�����݂���
//-----------------------------------------------------------------------------
bool CAIController::ExistsAvoidableSpace()
{
	return false;
}

//-----------------------------------------------------------------------------
// �p�X���ؒf����Ă��邩���ׂ�
//-----------------------------------------------------------------------------
bool CAIController::IsPathCutting()
{
	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	CMap* mapStage = modeGame->GetStage()->GetMap();

	for (int i = 0; i < (int)m_path.size(); i++)
	{
		if (mapStage->GetBlock(m_path.at(i).x, m_path.at(i).y)->GetType() != m_toOrder->GetTeam())
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// �����邽�߂Ɉړ�����
//-----------------------------------------------------------------------------
D3DXVECTOR3 CAIController::MoveToAvoid()
{
	return D3DXVECTOR3();
}

//-----------------------------------------------------------------------------
// �ǂ����߂Ɉړ�����
//-----------------------------------------------------------------------------
void CAIController::MoveToChase()
{
	ASTAR_PARAM status = SetAStarParam(m_toOrder->GetCenterBlock().at(0), m_enemy->GetCenterBlock().at(1));

	std::vector<POINT> routePlanX;
	bool isConnectX;
	if (SUCCEEDED(m_aStar->CalcPath(&status)))
	{
		DEBUG_PRINT("XPlanPath : Success\n");	// �f�o�b�O�\��
		isConnectX = true;
		routePlanX = m_aStar->GetPath();
	}
	else
	{
		DEBUG_PRINT("XPlanPath : Failure\n");	// �f�o�b�O�\��
		isConnectX = false;
	}

	if (routePlanX.size() > 15)
	{
		isConnectX = false;
	}

	status = SetAStarParam(m_enemy->GetCenterBlock().at(0), m_toOrder->GetCenterBlock().at(1));

	std::vector<POINT> routePlanY;
	bool isConnectY;
	if (SUCCEEDED(m_aStar->CalcPath(&status)))
	{
		DEBUG_PRINT("YPlanPath : Success\n");	// �f�o�b�O�\��
		isConnectY = true;
		routePlanY = m_aStar->GetPath();
	}
	else
	{
		DEBUG_PRINT("YPlanPath : Failure\n");	// �f�o�b�O�\��
		isConnectY = false;
	}

	if (routePlanY.size() > 15)
	{
		isConnectY = false;
	}

	if (!isConnectX && !isConnectY)
	{
		int distX = m_enemy->GetCenterBlock().at(0) - m_toOrder->GetCenterBlock().at(0);
		int distY = m_enemy->GetCenterBlock().at(1) - m_toOrder->GetCenterBlock().at(1);
		
		/* ���p�X�����Ȃ������ꍇ�� */
		if (m_toOrder->GetRemainsBullet() > 3)
		{
			if (std::abs(distX) < std::abs(distY))
			{
				if (distX <= 0)
				{
					m_shotType = LEFT_SHOT;
					DEBUG_PRINT("ChaseShot : LEFT_SHOT\n");	// �f�o�b�O�\��
				}
				else
				{
					m_shotType = RIGHT_SHOT;
					DEBUG_PRINT("ChaseShot : RIGHT_SHOT\n");	// �f�o�b�O�\��
				}
			}
			else
			{
				if (distY <= 0)
				{
					m_shotType = UP_SHOT;
					DEBUG_PRINT("ChaseShot : UP_SHOT\n");	// �f�o�b�O�\��
				}
				else
				{
					m_shotType = DOWN_SHOT;
					DEBUG_PRINT("ChaseShot : DOWN_SHOT\n");	// �f�o�b�O�\��
				}
			}
		}
		return;
	}

	/* ���p�X����ꂽ�ꍇ�� */

	if (isConnectX && !isConnectY)
	{
		/* ��X���̂݃p�X����ꂽ�ꍇ�� */

		SetMovePath(routePlanX);
	}
	if (!isConnectX && isConnectY)
	{
		/* ��Y���̂݃p�X����ꂽ�ꍇ�� */
		SetMovePath(routePlanY);
	}
	if (isConnectX && isConnectY)
	{
		/* �������̃p�X����ꂽ�ꍇ�� */

		if (routePlanX.size() <= routePlanY.size())
		{
			SetMovePath(routePlanX);
		}
		else
		{
			SetMovePath(routePlanY);
		}
	}
}

//-----------------------------------------------------------------------------
// �����L���邽�߂ɒe������
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToSpreadWay()
{
	return NONE_SHOT;
}

//-----------------------------------------------------------------------------
// �L�����N�^�[���U�����邽�߂ɒe������
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToAttack()
{
	if (m_attackCoolDownCount < 15)
	{
		m_attackCoolDownCount++;
		return NONE_SHOT;
	}

	m_attackCoolDownCount = 0;

	int distX = m_enemy->GetCenterBlock().at(0) - m_toOrder->GetCenterBlock().at(0);
	int distY = m_enemy->GetCenterBlock().at(1) - m_toOrder->GetCenterBlock().at(1);

	if (distX == 0)
	{
		if (m_enemy->GetCenterBlock().at(1) < m_toOrder->GetCenterBlock().at(1))
		{
			return UP_SHOT;
		}
		else
		{
			return DOWN_SHOT;
		}
	}

	if (distY == 0)
	{
		if (m_enemy->GetCenterBlock().at(0) < m_toOrder->GetCenterBlock().at(0))
		{
			return LEFT_SHOT;
		}
		else
		{
			return RIGHT_SHOT;
		}
	}
	return NONE_SHOT;
}

//-----------------------------------------------------------------------------
// �e�𑊎E���邽�߂ɒe������
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToOffsetBullet()
{
	static int count = 0;	// �e���˂̊Ԋu�B

	if (m_hitBullet == nullptr)
	{
		return NONE_SHOT;
	}

	if (isBulletShot)
	{
		count++;
		if (count >= 15)
		{
			isBulletShot = false;
			count = 0;
		}
		return NONE_SHOT;
	}

	std::vector<std::vector<int>> ofBlockCharcter = m_toOrder->GetOfBlock();	// �L�����N�^�[������Ă�u���b�N
	std::vector<std::vector<int>> ofBlockTarget = m_hitBullet->GetOfBlock();	// �e������Ă�u���b�N

	for (int i = 0; i < (int)ofBlockCharcter.size(); i++)
	{
		if (ofBlockCharcter.at(i).empty())
		{
			continue;
		}

		for (int j = 0; j < (int)ofBlockTarget.size(); j++)
		{
			if (ofBlockTarget.at(j).empty())
			{
				continue;
			}

			bool isXAxisMatched = ofBlockCharcter.at(i).at(0) == ofBlockTarget.at(j).at(0);	// X���̈�v
			bool isYAxisMatched = ofBlockCharcter.at(i).at(1) == ofBlockTarget.at(j).at(1);	// Y���̈�v

			if (isXAxisMatched)
			{
				if ((ofBlockCharcter.at(i).at(1) - 5 < ofBlockTarget.at(j).at(1)) && (ofBlockCharcter.at(i).at(1) > ofBlockTarget.at(j).at(1)))
				{
					isBulletShot = true;
					return UP_SHOT;
				}
				else if ((ofBlockCharcter.at(i).at(1) + 5 > ofBlockTarget.at(j).at(1)) && (ofBlockCharcter.at(i).at(1) < ofBlockTarget.at(j).at(1)))
				{
					isBulletShot = true;
					return DOWN_SHOT;
				}
			}
			else if (isYAxisMatched)
			{
				if ((ofBlockCharcter.at(i).at(0) - 5 < ofBlockTarget.at(j).at(0)) && (ofBlockCharcter.at(i).at(0) > ofBlockTarget.at(j).at(0)))
				{
					isBulletShot = true;
					return LEFT_SHOT;
				}
				else if ((ofBlockCharcter.at(i).at(0) + 5 > ofBlockTarget.at(j).at(0)) && (ofBlockCharcter.at(i).at(0) < ofBlockTarget.at(j).at(0)))
				{
					isBulletShot = true;
					return RIGHT_SHOT;
				}
			}
		}
	}
	return NONE_SHOT;
}
