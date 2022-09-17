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

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, __FILE__, __LINE__, NULL, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CAIController::CAIController() : 
	isBulletShot(false),
	m_aStar(nullptr),
	m_isCellMove(false),
	m_shotType(NONE_SHOT)
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
}

//-----------------------------------------
// �ړ�
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

		DEBUG_PRINT("Order : x = %d,y = %d\n", m_toOrder->GetCenterBlock()[0], m_toOrder->GetCenterBlock()[1]);	// �ړ�����f�o�b�O�\��
		for (int i = 0; i < (int)m_path.size(); i++)
		{
			bool isCenterHit = (m_path[i].x == m_toOrder->GetCenterBlock()[0]) && (m_path[i].y == m_toOrder->GetCenterBlock()[1]);

			DEBUG_PRINT("path  : x = %d,y = %d\n", m_path[i].x, m_path[i].y);	// �������|�����p�X���f�o�b�O�\��

			if (!(isCenterHit))
			{
				continue;
			}

			DEBUG_PRINT("success : x = %d,y = %d\n", m_toOrder->GetCenterBlock()[0], m_toOrder->GetCenterBlock()[1]);	// �o�͂Ƀf�o�b�O�\��
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
// �e�𔭎˂��鏈��
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
{
	static int count = 0;	// �e���˂̊Ԋu�B
	CCharacter* charcter = m_toOrder;
	std::vector<std::vector<int>> ofBlockCharcter = charcter->GetOfBlock();

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

			// �Ώۏ����̃u���b�N�擾
			std::vector<std::vector<int>> ofBlockTarget = bullet->GetOfBlock();

			// ���g���������Ă���u���b�N��S���`�F�b�N����
			for (int i = 0; i < 4; i++)
			{
				if (ofBlockCharcter[i].empty())
				{
					continue;
				}

				/* �������u���b�N���������ꍇ�� */

				// �Ώۂ��������Ă���u���b�N��S���`�F�b�N����
				for (int j = 0; j < 4; j++)
				{
					if (ofBlockTarget[j].empty())
					{
						continue;
					}

					/* �������u���b�N���������ꍇ�� */

					bool isXAxisMatched = ofBlockCharcter[i][0] == ofBlockTarget[j][0];	// X���̈�v
					bool isYAxisMatched = ofBlockCharcter[i][1] == ofBlockTarget[j][1];	// Y���̈�v

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
// �����邽�߂Ɉړ�����
//-----------------------------------------------------------------------------
D3DXVECTOR3 CAIController::MoveToAvoid()
{
	return D3DXVECTOR3();
}

//-----------------------------------------------------------------------------
// �ǂ����߂Ɉړ�����
//-----------------------------------------------------------------------------
D3DXVECTOR3 CAIController::MoveToChase()
{
	return D3DXVECTOR3();
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
	return NONE_SHOT;
}

//-----------------------------------------------------------------------------
// �e�𑊎E���邽�߂ɒe������
//-----------------------------------------------------------------------------
CController::SHOT_TYPE CAIController::ShootToOffsetBullet()
{
	return NONE_SHOT;
}
