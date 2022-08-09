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
#include "game.h"
#include "mode.h"
#include "map.h"
#include "input_keybord.h"
#include "bullet.h"
#include "controller.h"
#include "remains_bullet.h"
#include "collision.h"
#include "block.h"
#include <assert.h>

//-----------------------------------------
// 定義
//-----------------------------------------
const int CCharacter::LIMIT_BULLET_COUNT = 5;
const int CCharacter::RELOAD_TIME = 100;

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCharacter::CCharacter(CObject::TYPE type) :
	CObject2D(type),
	m_controller(nullptr),
	m_remainsBulletCount(0),
	m_reloadCount(0)
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
	SetPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);
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
	Move();	// 移動

	BulletShot();	// 弾の発射

	ScreenFromOutTime();	// スクリーン外に出た時

	// ブロックとの当たり判定
	Collision();

	// 弾数の回復
	if (m_remainsBulletCount < LIMIT_BULLET_COUNT)
	{
		m_reloadCount++;

		if (m_reloadCount % RELOAD_TIME == 0)
		{
			m_reloadCount = 0;
			m_remainsBulletCount++;
			m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(1.0f);
		}
	}
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
	m_move = m_controller->Move() * 4.0f;
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
		Shot(D3DXVECTOR3(0.0f, -10.0f, 0.0f));
		break;
	case CController::DOWN_SHOT:
		Shot(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		break;
	case CController::LEFT_SHOT:
		Shot(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
		break;
	case CController::RIGHT_SHOT:
		Shot(D3DXVECTOR3(10.0f, 0.0f, 0.0f));
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
		CObject2D::SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));		// 位置の設定
		break;
	case TEAM_01:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 位置の設定
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
	for (int i = 0; i < 4; i++)
	{
		if(m_ofBlockIndex[i] == inIndex)
		{
			return false;
		}
	}
	m_ofBlockIndex[count] = inIndex;
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
// スクリーン外に出た時
//-----------------------------------------
void CCharacter::ScreenFromOutTime()
{
	if (m_pos.x + size.x <= 0.0f - size.x)
	{
		m_pos.x = static_cast<float>(CApplication::GetInstance()->SCREEN_WIDTH);
		SetPos(m_pos);
	}
	if (m_pos.x - size.x >= CApplication::GetInstance()->SCREEN_WIDTH + size.x)
	{
		m_pos.x = 0.0f;
		SetPos(m_pos);
	}
	if (m_pos.y + size.y <= 0.0f - size.y)
	{
		m_pos.y = static_cast<float>(CApplication::GetInstance()->SCREEN_HEIGHT);
		SetPos(m_pos);
	}
	if (m_pos.y - size.y >= CApplication::GetInstance()->SCREEN_HEIGHT + size.y)
	{
		m_pos.y = 0.0f;
		SetPos(m_pos);
	}
}

//-----------------------------------------
// 当たり判定
//-----------------------------------------
void CCharacter::Collision()
{
	m_ofBlockCount = 0;

	auto HitBlock = [this](int x, int y)
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetMap();

		bool isHit = HitWithBlock(pMap->GetBlock(x, y));	// 当たったか否か

		if (isHit)
		{ // 当たった場合

			pMap->GetBlock(x, y)->SetRidingObject(this);	// ブロック側に自身を保存する
			if (SetBlockIndex(m_ofBlockCount, { x, y }))
			{ // 設定出来た場合
				m_ofBlockCount++;
			}
		}
	};

	for (int i = 0; i < 4;i++)
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

		// ブロック端の場合の処理
		if (LeftX < 0)
		{
			LeftX = 31;
		}
		if (RightX > 31)
		{
			RightX = 0;
		}
		if (TopY < 0)
		{
			TopY = 17;
		}
		if (BottomY > 17)
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

	if ((int)m_team == blockType)
	{
		if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(size.x, size.y, 0.0f), block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f))
		{
			return true;
		}
		return false;
	}

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float dist;

	if (m_move.y > 0.0f)
	{
		// プレイヤー上、ブロック下の当たり判定
		if (Collision::RectangleTop(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.y += 1.0f;
			dist = (size.y) * 0.5f + (m_pos.y - outpos.y);
			m_pos.y -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// 位置の設定
		}
	}
	if (m_move.x > 0.0f)
	{
		// プレイヤー右、ブロック左の当たり判定
		if (Collision::RectangleLeft(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.x += 1.0f;
			dist = (size.x) * 0.5f + (m_pos.x - outpos.x);
			m_pos.x -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// 位置の設定
		}
	}
	if (m_move.x < 0.0f)
	{
		// プレイヤー左、ブロック右の当たり判定
		if (Collision::RectangleRight(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.x += -1.0f;
			dist = (-size.x) * 0.5f + (m_pos.x - outpos.x);
			m_pos.x -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// 位置の設定
		}
	}
	if (m_move.y < 0.0f)
	{
		// プレイヤー下、ブロック上の当たり判定
		if (Collision::RectangleDown(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.y = -1.0f;
			dist = (-size.y) * 0.5f + (m_pos.y - outpos.y);
			m_pos.y -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// 位置の設定
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

	if ((int)m_team == (int)pBullet->CBullet::GetTeam())
	{
		return;
	}

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 弾のサイズ
	D3DXVECTOR3 bulletSize = D3DXVECTOR3(pBullet->GetSize().x, pBullet->GetSize().y, 0.0f) * 0.5f;

	// 自分のサイズ
	D3DXVECTOR3 mySize = D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f;

	// プレイヤー上、ブロック下の当たり判定
	if (Collision::RectangleTop(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleLeft(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleRight(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleDown(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL))
	{
		m_isDeleted = true;
	}
}
