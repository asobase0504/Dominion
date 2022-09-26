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
#include "map.h"
#include "bullet.h"
#include "controller.h"
#include "remains_bullet.h"
#include "stage.h"
#include "collision.h"
#include "block.h"
#include "crush_effect.h"
#include "sound.h"
#include <assert.h>
#include <functional>

//-----------------------------------------
// 定義
//-----------------------------------------
const int CCharacter::LIMIT_BULLET_COUNT = 5;	// 弾の最大数
const int CCharacter::RELOAD_TIME = 60;			// 一発の弾が回復する時間
const float CCharacter::MOVE_SPEAD = 5.0f;		// 移動速度

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCharacter::CCharacter(CObject::TYPE type) :
	CObject2D(type),
	m_stage(nullptr),
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
	assert(m_remainsBulletDisplay.empty());
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

	m_remainsBulletDisplay.clear();

	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCharacter::Update()
{
	if (m_isDeleted)
	{
		return;
	}

	if (!m_isOperationState)
	{
		return;
	}

	m_controller->Update();

	m_spead = MOVE_SPEAD;
	// 移動
	Move();

	// 弾の発射
	BulletShot();

	// 弾数の回復
	BulletReload();

	// 当たり判定
	Collision();

	m_pos += m_move;
	CObject2D::SetPos(m_pos);		// 位置の設定

	{ // 自身が所属しているブロックに軌跡を表示

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		pBlock->SetAdditionColor();
	}

	/*
	// バグの温床のため未実装に変更。時間があったらここから解決していきたい。
	*/
	// スクリーン外に出た時
	//ScreenFromOutTime();
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

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);

		CBullet* bullet = CBullet::Create(pBlock->GetPos(), inMove, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	};

	// 弾の発射
	switch (m_controller->BulletShot())
	{
	case CController::UP_SHOT:
		Shot(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		break;
	case CController::DOWN_SHOT:
		Shot(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		break;
	case CController::LEFT_SHOT:
		Shot(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
		break;
	case CController::RIGHT_SHOT:
		Shot(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		break;
	case CController::UP_CHARGE_SHOT:
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1]);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1]);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
		break;
	case CController::DOWN_CHARGE_SHOT:
	{

		CMap* pMap = m_stage->GetMap();

		// 弾を出現させるブロック
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1]);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1]);

		D3DXVECTOR3 move(0.0f, 1.0f, 0.0f);	// 移動量

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
		break;
	case CController::LEFT_CHARGE_SHOT:
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] - 1);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] + 1);
		D3DXVECTOR3 move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
	break;
	case CController::RIGHT_CHARGE_SHOT:
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] - 1);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1] + 1);
		D3DXVECTOR3 move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		CBullet* bullet;
		bullet = CBullet::Create(pBlock->GetPos(), move, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		if (pBlockUp->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockUp->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1] });
		}
		if (pBlockDown->GetType() != CBlock::BLOCK_TYPE::NONE)
		{
			bullet = CBullet::Create(pBlockDown->GetPos(), move, m_team, pMap);
			bullet->SetBlockIndex(0, { m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1] });
		}

		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	}
	break;
	case CController::CHARGE_NOW:
		m_spead *= 0.525f;
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
	m_isOperationState = true;
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
// ステージの設定
//-----------------------------------------
void CCharacter::SetStage(CStage * inStage)
{
	m_stage = inStage;
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

		// 回復した弾を残弾数に表記する
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
		for (int i = 0; i < (int)m_ofBlockIndex.size();i++)
		{
			character->SetBlockIndex(i, m_ofBlockIndex[i]);
		}

		// 中心の位置がどのブロックに所属しているかチェックする
		for (int i = 0; i < (int)character->m_ofBlockIndex.size(); i++)
		{
			if (character->m_ofBlockIndex[i].empty())
			{
				continue;
			}

			int x = character->m_ofBlockIndex[i][0];
			int y = character->m_ofBlockIndex[i][1];

			// そのブロックがキャラクターの中心が所属してるブロックがチェック
	
			CBlock* block = m_stage->GetMap()->GetBlock(x, y);
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
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEAD);
	}
}

//-----------------------------------------
// 当たり判定
//-----------------------------------------
void CCharacter::Collision()
{
	for (int i = 0; i < (int)m_ofBlockIndex.size(); i++)
	{
		m_ofBlockIndex[i].clear();
	}

	m_ofBlockCount = 0;

	// 一つのブロックとの当たり判定処理
	auto HitBlock = [this](int x, int y, DIRECTION inDirection, std::vector<DIRECTION> inAround)
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* block = pMap->GetBlock(x, y);

		if ((int)m_team == (int)block->CBlock::GetType())
		{
			/* ↓同じチームのブロックだったら↓ */

			if (!HitWithBlock(block))
			{
				return;
			}

			/* ↓ブロックに接触していたら↓ */

			pMap->GetBlock(x, y)->SetRidingObject(this);	// ブロック側に自身を保存する
			SetBlockIndex(m_ofBlockCount, { x, y });

			// そのブロックがキャラクターの中心が所属してるブロックがチェック
			block = pMap->GetBlock(x, y);
			D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f);	// ブロックの大きさ

			D3DXVECTOR3 movePlanPos = m_pos + m_move;

			if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
			{
				/* ↓キャラクターの中心が所属していた場合↓ */
				m_ofBlockIndexCenter = { x, y };
			}
		}
		else
		{
			/* ↓別チームのブロックだったら↓ */

			HitWithAnotherTeamBlock(block, inDirection, inAround);
		}
	};

	{ // 所属していないブロックとの当たり判定
		int CenterX = m_ofBlockIndexCenter[0];
		int LeftX = m_ofBlockIndexCenter[0] - 1;
		int RightX = m_ofBlockIndexCenter[0] + 1;

		int CenterY = m_ofBlockIndexCenter[1];
		int TopY = m_ofBlockIndexCenter[1] - 1;
		int BottomY = m_ofBlockIndexCenter[1] + 1;


		CMap* pMap = m_stage->GetMap();

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
		HitBlock(CenterX, CenterY, CenterCenter, inAround);		// 真ん中
		HitBlock(RightX, CenterY, RightCenter, inAround);	// 右真ん中
		HitBlock(LeftX, TopY, LeftTop, inAround);			// 左上
		HitBlock(RightX, TopY, RightTop, inAround);			// 右上
		HitBlock(LeftX, BottomY, LeftBottom, inAround);		// 左下
		HitBlock(RightX, BottomY, RightBottom, inAround);	// 右下

		D3DXVec3Normalize(&m_move, &m_move);
		m_move *= m_spead;
	}

	{

		CBlock* block = m_stage->GetMap()->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		
		if ((int)m_team != (int)block->CBlock::GetType())
		{
			for (int i = 0; i < 40; i++)
			{
				D3DXVECTOR3 pos;
				pos.x = m_pos.x + ((rand() / (float)RAND_MAX) * (40.0f - -40.0f)) + -40.0f;
				pos.y = m_pos.y + ((rand() / (float)RAND_MAX) * (40.0f - -40.0f)) + -40.0f;
				pos.z = 0.0f;
				D3DXVECTOR3 move;
				move.x = ((rand() / (float)RAND_MAX) * (40.0f - -40.0f)) + -40.0f;
				move.y = ((rand() / (float)RAND_MAX) * (40.0f - -40.0f)) + -40.0f;
				move.z = 0.0f;
				CCrushEffect::Create(pos, move, CApplication::GetInstance()->GetColor(m_team), block->CBlock::GetType(),m_stage->GetMap());
			}

			m_isDeleted = true;	// 死亡状態にする
			CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEAD);

		}
	}
}

//-----------------------------------------
// ブロックとの当たり判定
//-----------------------------------------
bool CCharacter::HitWithBlock(CBlock* inBlock)
{
	D3DXVECTOR3 movePlanPos = m_pos + m_move;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(inBlock->GetSize().x, inBlock->GetSize().y, 0.0f);	// ブロックの大きさ

	if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(m_size.x, m_size.y, 0.0f), inBlock->GetPos(), blockSize))
	{
		return true;
	}
	return false;
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
		for (int i = 0; i < (int)inAround.size(); i++)
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
		for (int i = 0; i < (int)inAround.size(); i++)
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
		for (int i = 0; i < (int)inAround.size(); i++)
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
		for (int i = 0; i < (int)inAround.size(); i++)
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
