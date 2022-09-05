//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "character.h"
#include "application.h"
#include "mode.h"
#include "game.h"
#include "map.h"
#include "bullet.h"
#include "controller.h"
#include "remains_bullet.h"
#include "stage.h"
#include "collision.h"
#include "block.h"
#include <assert.h>
#include <functional>

//-----------------------------------------
// 定義
//-----------------------------------------
const int CCharacter::LIMIT_BULLET_COUNT = 5;	// 弾の最大数
const int CCharacter::RELOAD_TIME = 100;		// 一発の弾が回復する時間
const float CCharacter::MOVE_SPEAD = 4.0f;		// 移動速度

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCharacter::CCharacter(CObject::TYPE type) :
	CObject2D(type),
	m_controller(nullptr),
	m_remainsBulletCount(0),
	m_reloadCount(0),
	isCopied(false)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CCharacter::~CCharacter()
{
	//assert(m_controller == nullptr);
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	m_remainsBulletCount = LIMIT_BULLET_COUNT;
	m_ofBlockIndex.resize(4);
	SetTexture("PLAYER");	// テクスチャの指定
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CCharacter::Uninit()
{
	for (auto it = m_remainsBulletDisplay.begin(); it != m_remainsBulletDisplay.end(); it++)
	{
		(*it)->SetIsDeleted(true);
	}

	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCharacter::Update()
{
	m_spead = MOVE_SPEAD;
	// 移動
	Move();

	// 弾の発射
	BulletShot();

	// 弾数の回復
	BulletReload();

	// ブロックとの当たり判定
	Collision();

	m_pos += m_move;
	CObject2D::SetPos(m_pos);		// 位置の設定

	CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
	CMap* pMap = game->GetStage()->GetMap();
	CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
	pBlock->SetAdditionColor();

	// スクリーン外に出た時
	ScreenFromOutTime();
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CCharacter::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 移動
//-----------------------------------------
void CCharacter::Move()
{
	if (m_controller == nullptr)
	{
		assert(false);
		return;
	}

	// 方向ベクトル掛ける移動量
	m_move = m_controller->Move() * MOVE_SPEAD;
}

//-----------------------------------------
// 弾の発射
//-----------------------------------------
void CCharacter::BulletShot()
{
	if (m_controller == nullptr)
	{
		assert(false);
		return;
	}

	// 残弾数が0だったら。
	if (m_remainsBulletCount <= 0)
	{
		return;
	}

	// 弾を撃ちだすラムダ式
	auto Shot = [this](const D3DXVECTOR3& inMove)
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);

		CBullet* bullet = CBullet::Create(pBlock->GetPos(), inMove, m_team);
		bullet->SetBlockIndex(0, m_ofBlockIndex[0]);
		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	};

	// 弾の発射
	switch (m_controller->BulletShot())
	{
	case CController::UP_SHOT:
		Shot(D3DXVECTOR3(0.0f, -(CBullet::MOVE_SPEAD), 0.0f));
		break;
	case CController::DOWN_SHOT:
		Shot(D3DXVECTOR3(0.0f, CBullet::MOVE_SPEAD, 0.0f));
		break;
	case CController::LEFT_SHOT:
		Shot(D3DXVECTOR3(-(CBullet::MOVE_SPEAD), 0.0f, 0.0f));
		break;
	case CController::RIGHT_SHOT:
		Shot(D3DXVECTOR3(CBullet::MOVE_SPEAD, 0.0f, 0.0f));
		break;
	case CController::UP_CHARGE_SHOT:
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1]);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1]);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -(CBullet::MOVE_SPEAD), 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team);
		bullet->SetBlockIndex(0, m_ofBlockIndex[0]);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
		break;
	case CController::DOWN_CHARGE_SHOT:
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1]);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1]);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, CBullet::MOVE_SPEAD, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team);
		bullet->SetBlockIndex(0, m_ofBlockIndex[0]);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
		break;
	case CController::LEFT_CHARGE_SHOT:
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] - 1);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] + 1);
		D3DXVECTOR3 move = D3DXVECTOR3(-CBullet::MOVE_SPEAD, 0.0f, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team);
		bullet->SetBlockIndex(0, m_ofBlockIndex[0]);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
	break;
	case CController::RIGHT_CHARGE_SHOT:
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] - 1);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] + 1);
		D3DXVECTOR3 move = D3DXVECTOR3(CBullet::MOVE_SPEAD, 0.0f, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team);
		bullet->SetBlockIndex(0, m_ofBlockIndex[0]);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
	break;
	case CController::CHARGE_NOW:
		m_spead *= 0.5f;
		break;
	default:
		break;
	}
}

//-----------------------------------------
// コントローラーの設定
//-----------------------------------------
void CCharacter::SetController(CController * inOperate)
{
	m_controller = inOperate;
	m_controller->SetToOrder(this);
}

//-----------------------------------------
// チームの設定
//-----------------------------------------
void CCharacter::SetTeam(const TEAM inTeam)
{
	m_team = inTeam;

	switch (m_team)
	{
	case TEAM_00:
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(1));	// 色の設定
		break;
	case TEAM_01:
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(0));	// 色の設定
		break;
	default:
		break;
	}
}

//-----------------------------------------
// 乗ってるブロックの番号を設定
//-----------------------------------------
bool CCharacter::SetBlockIndex(const int count, std::vector<int> inIndex)
{
	// 空だった場合
	if (inIndex.empty())
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		if(m_ofBlockIndex[i] == inIndex)
		{
			return false;
		}
	}

	// 追加
	m_ofBlockIndex[count] = inIndex;
	m_ofBlockCount++;
	return true;
}

//-----------------------------------------
// 生成
//-----------------------------------------
CCharacter* CCharacter::Create(TEAM team)
{
	CCharacter* player = new CCharacter;

	if (player == nullptr)
	{
		return nullptr;
	}

	player->Init();
	player->SetTeam(team);
	float rot = D3DX_PI * 2.0f / LIMIT_BULLET_COUNT;
	for (int i = 0; i < LIMIT_BULLET_COUNT; i++)
	{
		player->m_remainsBulletDisplay.push_back(CRemaubsBullet::Create(player, rot * i));
	}
	return player;
}

//-----------------------------------------
// 弾のリロード処理
//-----------------------------------------
void CCharacter::BulletReload()
{
	if (m_remainsBulletCount < LIMIT_BULLET_COUNT)
	{
		m_reloadCount++;

		if (m_reloadCount % (int)(RELOAD_TIME * 0.5f) == 0)
		{
			m_remainsBulletDisplay[m_remainsBulletCount]->SetColorAlpha(0.5f);
		}

		if (m_reloadCount % RELOAD_TIME == 0)
		{
			m_reloadCount = 0;
			m_remainsBulletCount++;
			m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(1.0f);
		}
	}
}

//-----------------------------------------
// スクリーン外に出た時
//-----------------------------------------
void CCharacter::ScreenFromOutTime()
{
	// 自身の複製
	auto Copy = [this](D3DXVECTOR3 inPos)
	{
		if (isCopied)
		{
			return;
		}

		CCharacter* character = Create(m_team);
		character->SetPos(inPos);
		character->SetSize(m_size);		// 大きさの設定

		// ブロックの番号登録
		for (int i = 0; i < m_ofBlockIndex.size();i++)
		{
			character->SetBlockIndex(i, m_ofBlockIndex[i]);
		}

		// 中心の位置がどのブロックに所属しているかチェックする
		for (int i = 0; i < character->m_ofBlockIndex.size(); i++)
		{
			if (character->m_ofBlockIndex[i].empty())
			{
				continue;
			}

			int x = character->m_ofBlockIndex[i][0];
			int y = character->m_ofBlockIndex[i][1];

			// そのブロックがキャラクターの中心が所属してるブロックがチェック
			CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
			CBlock* block = game->GetStage()->GetMap()->GetBlock(x, y);
			D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f);	// ブロックの大きさ

			if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
			{
				character->m_ofBlockIndexCenter = { x, y };
			}
		}

		character->SetController(m_controller);	// 命令者の設定
		isCopied = true;			// コピー済みにする
		character->isCopied = true;	// コピー先をコピー済みにする
	};

	if (m_pos.x - m_size.x * 0.5f <= 0.0f)
	{
		D3DXVECTOR3 pos(CApplication::GetInstance()->SCREEN_WIDTH + m_size.x * 0.5f, m_pos.y, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.x + m_size.x * 0.5f >= CApplication::GetInstance()->SCREEN_WIDTH)
	{
		D3DXVECTOR3 pos(0.0f - m_size.x * 0.5f, m_pos.y, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.y - m_size.y * 0.5f <= 0.0f)
	{
		D3DXVECTOR3 pos(m_pos.x, CApplication::GetInstance()->SCREEN_HEIGHT + m_size.y * 0.5f, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.y + m_size.y * 0.5f >= CApplication::GetInstance()->SCREEN_HEIGHT)
	{
		D3DXVECTOR3 pos(m_pos.x, 0.0f - m_size.y * 0.5f, m_pos.z);
		Copy(pos);
	}
	else
	{
		isCopied = false;
	}

	float dist = 0.00001f;	// 消える場所と出現位置をずらすための値
	if (m_pos.x + m_size.x * 0.5f + dist <= 0.0f ||
		m_pos.y + m_size.y * 0.5f + dist <= 0.0f ||
		m_pos.x - m_size.x * 0.5f - dist >= CApplication::GetInstance()->SCREEN_WIDTH ||
		m_pos.y - m_size.y * 0.5f - dist >= CApplication::GetInstance()->SCREEN_HEIGHT)
	{
		// 自身の削除
		m_isDeleted = true;
	}
}

//-----------------------------------------
// 当たり判定
//-----------------------------------------
void CCharacter::Collision()
{
	m_ofBlockCount = 0;

	// 一つのブロックとの当たり判定処理
	auto HitBlock = [this](int x, int y, DIRECTION inDirection, std::vector<DIRECTION> inAround)
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();
		CBlock* block = pMap->GetBlock(x, y);

		if ((int)m_team == (int)block->CBlock::GetType())
		{
			if (!HitWithBlock(block))
			{
				return;
			}

			pMap->GetBlock(x, y)->SetRidingObject(this);	// ブロック側に自身を保存する
			SetBlockIndex(m_ofBlockCount, { x, y });

			// そのブロックがキャラクターの中心が所属してるブロックがチェック
			CBlock* block = pMap->GetBlock(x, y);
			D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f);	// ブロックの大きさ

			D3DXVECTOR3 movePlanPos = m_pos + m_move;
			if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
			{
				m_ofBlockIndexCenter = { x, y };
			}
		}
		else
		{
			// 別チームとの当たり判定
			HitWithAnotherTeamBlock(block, inDirection, inAround);
		}
	};

	{
		int CenterX = m_ofBlockIndexCenter[0];
		int LeftX = m_ofBlockIndexCenter[0] - 1;
		int RightX = m_ofBlockIndexCenter[0] + 1;

		int CenterY = m_ofBlockIndexCenter[1];
		int TopY = m_ofBlockIndexCenter[1] - 1;
		int BottomY = m_ofBlockIndexCenter[1] + 1;

		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetStage()->GetMap();

		// ブロック端の場合の処理
		if (LeftX < 0)
		{
			LeftX = pMap->GetMaxXBlock() - 1;
		}
		if (RightX > pMap->GetMaxXBlock() - 1)
		{
			RightX = 0;
		}
		if (TopY < 0)
		{
			TopY = pMap->GetMaxYBlock() - 1;
		}
		if (BottomY > pMap->GetMaxYBlock() - 1)
		{
			BottomY = 0;
		}

		// ブロックの当たり判定
		std::vector<DIRECTION> inAround;

		CBlock* block = pMap->GetBlock(CenterX, TopY);
		if ((int)m_team != (int)block->CBlock::GetType())
		{
			inAround.push_back(CenterTop);
		}
		block = pMap->GetBlock(CenterX, BottomY);
		if ((int)m_team != (int)block->CBlock::GetType())
		{
			inAround.push_back(CenterBottom);
		}
		block = pMap->GetBlock(LeftX, CenterY);
		if ((int)m_team != (int)block->CBlock::GetType())
		{
			inAround.push_back(LeftCenter);
		}
		block = pMap->GetBlock(RightX, CenterY);
		if ((int)m_team != (int)block->CBlock::GetType())
		{
			inAround.push_back(RightCenter);
		}

		HitBlock(CenterX, TopY, CenterTop, inAround);		// 上
		HitBlock(CenterX, BottomY, CenterBottom, inAround);	// 下
		HitBlock(LeftX, CenterY, LeftCenter, inAround);		// 左真ん中
		HitBlock(RightX, CenterY, RightCenter, inAround);	// 右真ん中
		HitBlock(LeftX, TopY, LeftTop, inAround);			// 左上
		HitBlock(RightX, TopY, RightTop, inAround);			// 右上
		HitBlock(LeftX, BottomY, LeftBottom, inAround);		// 左下
		HitBlock(RightX, BottomY, RightBottom, inAround);	// 右下

		D3DXVec3Normalize(&m_move, &m_move);
		m_move *= m_spead;
	}

	// 弾との当たり判定
	for (auto it = GetMyObject(2)->begin(); it != GetMyObject(2)->end(); it++)
	{
		if ((*it)->GetIsDeleted())
		{
			continue;
		}

		if ((*it)->CObject::GetType() == CObject::TYPE::BULLET)
		{
			HitWithBullet((CBullet*)(*it));
		}
	}
}

//-----------------------------------------
// ブロックとの当たり判定
//-----------------------------------------
bool CCharacter::HitWithBlock(CBlock* inBlock)
{
	D3DXVECTOR3 movePlanPos = m_pos + m_move;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(inBlock->GetSize().x, inBlock->GetSize().y, 0.0f) * 0.5f;	// ブロックの大きさ

	if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(m_size.x, m_size.y, 0.0f), inBlock->GetPos(), blockSize))
	{
		return true;
	}
	return false;
}

//-----------------------------------------
// 弾との当たり判定
//-----------------------------------------
void CCharacter::HitWithBullet(CBullet* inBullet)
{
	CBullet* pBullet = inBullet;

	// 同じ所属なら弾く
	if ((int)m_team == (int)pBullet->CBullet::GetTeam())
	{
		return;
	}

	std::vector<std::vector<int>> bulletOfBlock = pBullet->GetOfBlock();

	// 乗ってるブロックの数分回す
	for (int i = 0; i < m_ofBlockIndex.size(); i++)
	{
		if (m_ofBlockIndex[i].empty())
		{
			continue;
		}

		/* ↓プレイヤーの所属ブロックが空ではなかった場合↓ */

		for (int j = 0; j < bulletOfBlock.size(); j++)
		{
			if (bulletOfBlock[j].empty())
			{
				continue;
			}

			/* ↓弾の所属ブロックが空ではなかった場合↓ */

			if (m_ofBlockIndex[i] == bulletOfBlock[j])
			{
				m_isDeleted = true;
				return;
			}
		}
	}
}

//-----------------------------------------
// 別チームのブロックとの当たり判定
//-----------------------------------------
void CCharacter::HitWithAnotherTeamBlock(CBlock * inBlock, DIRECTION inDirection ,std::vector<DIRECTION> inAround)
{
	CBlock* block = inBlock;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// ブロックの大きさ

	std::function<bool(Collision::RECTANGLE_DIRECTION)> Collision;

	Collision = [this, block, blockSize, inDirection, inAround](Collision::RECTANGLE_DIRECTION inDirect)->bool
	{
		D3DXVECTOR3 movePlanPos = m_pos + m_move;
		D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// プレイヤー上、ブロック下の当たり判定
		if (!(Collision::RectangleSegment(inDirect,block->GetPos(), blockSize, movePlanPos, D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f, &outpos, NULL, NULL)))
		{
			return false;
		}

		switch (inDirect)
		{
		case Collision::RECTANGLE_DIRECTION::TOP:
			m_move.y *= 0.9f;
			if (m_move.y < 1.0f)
			{
				m_move.y = 0.0f;
				return true;
			}
			break;
		case Collision::RECTANGLE_DIRECTION::DOWN:
			m_move.y *= 0.9f;
			if (m_move.y > -1.0f)
			{
				m_move.y = 0.0f;
				return true;
			}
			break;
		case Collision::RECTANGLE_DIRECTION::LEFT:
			m_move.x *= 0.9f;
			if (m_move.x < 1.0f)
			{
				m_move.x = 0.0f;
				return true;
			}
			break;
		case Collision::RECTANGLE_DIRECTION::RIGHT:
			m_move.x *= 0.9f;
			if (m_move.x > -1.0f)
			{
				m_move.x = 0.0f;
				return true;
			}
			break;
		default:
			break;
		}

		HitWithAnotherTeamBlock(block, inDirection, inAround);
		return true;
	};

	switch (inDirection)
	{
	case LeftTop:	// 左上
	{
		bool top = false;
		bool left = false;
		for (int i = 0; i < inAround.size(); i++)
		{
			if (inAround[i] == CenterTop)
			{
				top = true;
			}
			if (inAround[i] == LeftCenter)
			{
				left = true;
			}
		}
		if (!top && !left && m_move.x < 0.0f && m_move.y < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (!top && !left && m_move.x < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		}
		else if (!top && !left && m_move.y < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (top && !left)
		{
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (!top && left)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		}
	}
		break;
	case CenterTop:	// 上
		Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		break;
	case RightTop:	// 右上
	{
		bool top = false;
		bool right = false;
		for (int i = 0; i < inAround.size(); i++)
		{
			if (inAround[i] == CenterTop)
			{
				top = true;
			}
			if (inAround[i] == RightCenter)
			{
				right = true;
			}
		}
		if (!top && !right && m_move.x > 0.0f && m_move.y < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (!top && !right && m_move.x > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		}
		else if (!top && !right && m_move.y < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (top && !right)
		{
			Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		}
		else if (!top && right)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		}
	}
		break;
	case LeftCenter:	// 左
		Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		break;
	case CenterCenter:	// 真ん中
		break;
	case RightCenter:	// 右
		Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		break;
	case LeftBottom:	// 左下
	{
		bool bottom = false;
		bool left = false;
		for (int i = 0; i < inAround.size(); i++)
		{
			if (inAround[i] == CenterBottom)
			{
				bottom = true;
			}
			if (inAround[i] == LeftCenter)
			{
				left = true;
			}
		}
		if (!bottom && !left && m_move.x < 0.0f && m_move.y > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (!bottom && !left && m_move.x < 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		}
		else if (!bottom && !left && m_move.y > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (bottom && !left)
		{
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (!bottom && left)
		{
			Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		}
	}
	break;
	case CenterBottom:	// 下
		Collision(Collision::RECTANGLE_DIRECTION::TOP);
		break;
	case RightBottom:	// 右下
	{
		bool bottom = false;
		bool right = false;
		for (int i = 0; i < inAround.size(); i++)
		{
			if (inAround[i] == CenterBottom)
			{
				bottom = true;
			}
			if (inAround[i] == RightCenter)
			{
				right = true;
			}
		}
		if (!bottom && !right && m_move.x > 0.0f && m_move.y > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (!bottom && !right && m_move.x > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		}
		else if (!bottom && !right && m_move.y > 0.0f)
		{
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (bottom && !right)
		{
			Collision(Collision::RECTANGLE_DIRECTION::TOP);
		}
		else if (!bottom && right)
		{
			Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		}
	}
	break;
	default:
		break;
	}
}
