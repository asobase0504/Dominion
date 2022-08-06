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
#include <vector>

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CAIController::CAIController() : 
	isBulletShot(false)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CAIController::~CAIController()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CAIController::Init()
{
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CAIController::Uninit()
{
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
	return D3DXVECTOR3(-0.5f,0.0f,0.0f);
}

//-----------------------------------------
// 弾を発射する処理
//-----------------------------------------
CController::SHOT_TYPE CAIController::BulletShot()
{
	static int count = 0;
	CCharacter* charcter = (CCharacter*)m_toOrder;
	std::vector<std::vector<int>> ofBlockCharcter = charcter->GetOfBlock();

	// オブジェクトを全てチェックする
	for (auto it = CObject::GetMyObject()->begin(); it != CObject::GetMyObject()->end(); it++)
	{
		if ((*it)->GetIsDeleted())
		{
			continue;
		}

		/* ↓オブジェクトが死ぬ予定がない場合↓ */

		if ((*it)->CObject::GetType() == CObject::TYPE::BULLET)
		{
			CBullet* bullet = (CBullet*)(*it);

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

			if ((int)bullet->GetTeam() == (int)charcter->GetType())
			{
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

					if (ofBlockCharcter[i][0] == ofBlockTarget[j][0])
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
					else if (ofBlockCharcter[i][1] == ofBlockTarget[j][1])
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
