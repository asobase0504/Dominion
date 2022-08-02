//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "bullet.h"
#include "application.h"
#include "collision.h"
#include "block.h"
#include "game.h"
#include "map.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CBullet::CBullet(CObject::TYPE type) :
	CObject2D(type),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_life(0)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CBullet::~CBullet()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CBullet::Init()
{
	CObject2D::Init();
	m_ofBlockIndex.resize(4);
	m_life = 200;	// 初期値
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CBullet::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// 体力の減少
	m_life--;

	// 位置の設定
	SetPos(GetPos() + m_move);

	// 当たり判定
	Collision();

	// スクリーン外の処理
	ScreenFromOutTime();

	// 死亡処理
	if (m_life <= 0)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// チームの設定
//-----------------------------------------
void CBullet::SetTeam(const CCharacter::TEAM inTeam)
{
	m_team = inTeam;

	switch (m_team)
	{
	case CCharacter::TEAM_00:
		CObject2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// 位置の設定
		break;
	case CCharacter::TEAM_01:
		CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 位置の設定
		break;
	default:
		break;
	}
}

bool CBullet::SetBlockIndex(const int count, std::vector<int> inIndex)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_ofBlockIndex[i] == inIndex)
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
CBullet* CBullet::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove, const CCharacter::TEAM inTeam)
{
	CBullet* bullet = new CBullet;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->SetPos(inPos);
	bullet->m_move = inMove;
	bullet->SetTeam(inTeam);
	bullet->SetSize(D3DXVECTOR2(10.0f, 10.0f));

	return bullet;
}

//-----------------------------------------
// 全体の当たり判定
//-----------------------------------------
void CBullet::Collision()
{
	m_ofBlockCount = 0;

	for (int i = 0; i < 4; i++)
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
		SetHitBlock(LeftX, TopY);			// 左上
		SetHitBlock(CenterX, TopY);			// 上
		SetHitBlock(RightX, TopY);			// 右上
		SetHitBlock(LeftX, CenterY);		// 左真ん中
		SetHitBlock(CenterX, CenterY);		// 真ん中
		SetHitBlock(RightX, CenterY);		// 右真ん中
		SetHitBlock(LeftX, BottomY);		// 左下
		SetHitBlock(CenterX, BottomY);		// 下
		SetHitBlock(RightX, BottomY);		// 右下
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

//------------------------------------------------------------------
// ブロックとの当たり判定呼び出しと、乗ってるブロックの設定
//------------------------------------------------------------------
void CBullet::SetHitBlock(int x, int y)
{
	CMap* pMap = CApplication::GetInstance()->GetGame()->GetMap();

	bool isHit = HitWithBlock(pMap->GetBlock(x, y));	// 当たったか否か

	if (isHit)
	{ // 当たった場合
		if (SetBlockIndex(m_ofBlockCount, { x, y }))
		{ // 設定出来た場合
			m_ofBlockCount++;
		}
	}
}

//-----------------------------------------
// スクリーン外に出た時
//-----------------------------------------
void CBullet::ScreenFromOutTime()
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
// 弾との当たり判定
//-----------------------------------------
void CBullet::HitWithBullet(CBullet* inBullet)
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
	if (Collision::CircleAndCircle(pBullet->GetPos(), D3DXVec3Length(&bulletSize), m_pos, D3DXVec3Length(&mySize)))
	{
		inBullet->m_isDeleted = true;
		m_isDeleted = true;
	}
}

//-----------------------------------------
// ブロックとの当たり判定
//-----------------------------------------
bool CBullet::HitWithBlock(CBlock * inBlock)
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

	D3DXVECTOR3 pairSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;
	D3DXVECTOR3 mySize = D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f;

	if (Collision::RectangleTop(block->GetPos(), pairSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleLeft(block->GetPos(), pairSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleRight(block->GetPos(), pairSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleDown(block->GetPos(), pairSize, m_pos, mySize, &outpos, NULL, NULL))
	{
		if (blockType == CBlock::NONE)
		{
			m_isDeleted = true;
			return false;
		}

		block->ChangeType();
		return true;
	}
	return false;
}
