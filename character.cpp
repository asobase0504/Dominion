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
#include "collision.h"
#include "block.h"
#include <assert.h>

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
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	m_remainsBulletCount = LIMIT_BULLET_COUNT;
	m_ofBlockIndex.resize(4);
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);	// テクスチャの指定
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CCharacter::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCharacter::Update()
{
	// 移動
	Move();

	// 弾の発射
	BulletShot();

	// 弾数の回復
	BulletReload();

	// スクリーン外に出た時
	ScreenFromOutTime();

	// ブロックとの当たり判定
	Collision();
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
	m_pos += m_move;
	CObject2D::SetPos(m_pos);		// 位置の設定
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

	auto Shot = [this](const D3DXVECTOR3& inMove)
	{
		CBullet* bullet = CBullet::Create(m_pos, inMove, m_team);
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
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// 色の設定
		break;
	case TEAM_01:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定
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
		character->SetSize(D3DXVECTOR2(25.0f, 25.0f));		// 大きさの設定

		// ブロックの番号登録
		for (int i = 0; i < m_ofBlockIndex.size();i++)
		{
			character->SetBlockIndex(i, m_ofBlockIndex[i]);
		}

		character->SetController(m_controller);	// 命令者の設定
		character->isCopied = true;	// コピー済みにする
		isCopied = true;	// コピー済みにする
	};

	if (m_pos.x - size.x <= 0.0f)
	{
		D3DXVECTOR3 pos(CApplication::GetInstance()->SCREEN_WIDTH + size.x, m_pos.y, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.x + size.x >= CApplication::GetInstance()->SCREEN_WIDTH)
	{
		D3DXVECTOR3 pos(0.0f - size.x, m_pos.y, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.y - size.y <= 0.0f)
	{
		D3DXVECTOR3 pos(m_pos.x, CApplication::GetInstance()->SCREEN_HEIGHT + size.y, m_pos.z);
		Copy(pos);
	}
	else if (m_pos.y + size.y >= CApplication::GetInstance()->SCREEN_HEIGHT)
	{
		D3DXVECTOR3 pos(m_pos.x, 0.0f - size.y, m_pos.z);
		Copy(pos);
	}
	else
	{
		isCopied = false;
	}

	float dist = 0.001f;	// 消える場所と出現位置をずらすための値
	if (m_pos.x + size.x + dist <= 0.0f - size.x ||
		m_pos.y + size.y + dist <= 0.0f - size.y ||
		m_pos.x - size.x - dist >= CApplication::GetInstance()->SCREEN_WIDTH + size.x ||
		m_pos.y - size.y - dist >= CApplication::GetInstance()->SCREEN_HEIGHT + size.y)
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
	auto HitBlock = [this](int x, int y)
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetMap();

		bool isHit = HitWithBlock(pMap->GetBlock(x, y));	// 当たったか否か

		if (!isHit)
		{ // 当たってない場合
			return;
		}

		/* ↓当たった場合↓ */
		pMap->GetBlock(x, y)->SetRidingObject(this);	// ブロック側に自身を保存する
		SetBlockIndex(m_ofBlockCount, { x, y });

		// そのブロックがキャラクターの中心が所属してるブロックがチェック
		CBlock* block = pMap->GetBlock(x, y);
		D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// ブロックの大きさ

		if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
		{
			m_ofBlockIndexCenter = { x, y };
		}
	};

	for (int i = 0; i < m_ofBlockIndex.size();i++)
	{
		if (m_ofBlockIndex[i].empty())
		{
			continue;
		}

		int CenterX = m_ofBlockIndex[i][0];
		int LeftX = m_ofBlockIndex[i][0] - 1;
		int RightX = m_ofBlockIndex[i][0] + 1;

		int CenterY = m_ofBlockIndex[i][1];
		int TopY = m_ofBlockIndex[i][1] - 1;
		int BottomY = m_ofBlockIndex[i][1] + 1;

		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetMap();

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
		HitBlock(LeftX, TopY);		// 左上
		HitBlock(CenterX, TopY);	// 上
		HitBlock(RightX, TopY);		// 右上
		HitBlock(LeftX, CenterY);	// 左真ん中
		HitBlock(CenterX, CenterY);	// 真ん中
		HitBlock(RightX, CenterY);	// 右真ん中
		HitBlock(LeftX, BottomY);	// 左下
		HitBlock(CenterX, BottomY);	// 下
		HitBlock(RightX, BottomY);	// 右下
	}

	for (auto it = GetMyObject()->begin(); it != GetMyObject()->end(); it++)
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
	CBlock* block = inBlock;

	int blockType = (int)block->CBlock::GetType();
	D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// ブロックの大きさ

	// 自分と同じ所属だった場合
	if ((int)m_team == blockType)
	{
		if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(size.x, size.y, 0.0f), block->GetPos(), blockSize))
		{
			return true;
		}
		return false;
	}

	/* ↓自分と違う所属だった場合↓ */

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float dist;

	if (m_move.y > 0.0f)
	{
		// プレイヤー上、ブロック下の当たり判定
		if (Collision::RectangleTop(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (size.y) * 0.5f + (m_pos.y - outpos.y);	// 差分
			m_pos.y -= dist + dist * 0.0001f;	// 位置の設定
			CObject2D::SetPos(m_pos);		// 位置の反映
		}
	}
	if (m_move.x > 0.0f)
	{
		// プレイヤー右、ブロック左の当たり判定
		if (Collision::RectangleLeft(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (size.x) * 0.5f + (m_pos.x - outpos.x);	// 差分
			m_pos.x -= dist + dist * 0.0001f;	// 位置の設定
			CObject2D::SetPos(m_pos);		// 位置の反映
		}
	}
	if (m_move.x < 0.0f)
	{
		// プレイヤー左、ブロック右の当たり判定
		if (Collision::RectangleRight(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (-size.x) * 0.5f + (m_pos.x - outpos.x);	// 差分
			m_pos.x -= dist + dist * 0.0001f;	// 位置の設定
			CObject2D::SetPos(m_pos);		// 位置の反映
		}
	}
	if (m_move.y < 0.0f)
	{
		// プレイヤー下、ブロック上の当たり判定
		if (Collision::RectangleDown(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (-size.y) * 0.5f + (m_pos.y - outpos.y);	// 差分
			m_pos.y -= dist + dist * 0.0001f;	// 位置の設定
			CObject2D::SetPos(m_pos);		// 位置の反映
		}
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
