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
#include <vector>

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CAIController::CAIController() : 
	isBulletShot(false)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CAIController::~CAIController()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CAIController::Init()
{
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CAIController::Uninit()
{
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
	return D3DXVECTOR3(-0.5f,0.0f,0.0f);
}

//-----------------------------------------
// �e�𔭎˂��鏈��
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
{
	static int count = 0;
	CCharacter* charcter = (CCharacter*)m_toOrder;
	std::vector<std::vector<int>> ofBlockCharcter = charcter->GetOfBlock();

	// �I�u�W�F�N�g��S�ă`�F�b�N����
	for (int i = 0; i < CObject::GetPrioritySize(); i++)
	{
		for (auto it = CObject::GetMyObject(i)->begin(); it != CObject::GetMyObject(i)->end(); it++)
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

			CBullet* bullet = (CBullet*)(*it);

			if ((int)bullet->GetTeam() == (int)charcter->GetType())
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
