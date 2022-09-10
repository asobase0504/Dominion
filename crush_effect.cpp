//=========================================
// 
// 砕けるエフェクト
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "crush_effect.h"
#include "application.h"
#include "stage.h"
#include "game.h"
#include "map.h"
#include "block.h"
#include "collision.h"

//-----------------------------------------
// 定義
//-----------------------------------------

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCrushEffect::CCrushEffect(CObject::TYPE type) :
	CObject2D(type, 2),
	m_life(0)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CCrushEffect::~CCrushEffect()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CCrushEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// 初期値
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CCrushEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCrushEffect::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// 体力の減少
	m_life--;

	//m_move *= 0.75f;

	CGame* modeGame = (CGame*)CApplication::GetInstance()->GetMode();
	CMap* cMap = modeGame->GetStage()->GetMap();
	int Xindex = cMap->GetMaxXBlock();
	int Yindex = cMap->GetMaxYBlock();

	for (int x = 0; x < Xindex ; x++)
	{
		for (int y = 0; y < Yindex; y++)
		{
			CBlock* block = cMap->GetBlock(x, y);

			if (block->GetType() == m_team)
			{
				continue;
			}

			if (Collision::RectangleAndRectangle(m_pos + m_move, (D3DXVECTOR3)m_size, block->GetPos(), (D3DXVECTOR3)block->GetSize()))
			{
				m_move *= -1.0f;
				m_move *= 0.65f;
			}
		}
	}

	SetPos(GetPos() + m_move);
	SetSize(GetSize() - D3DXVECTOR2(0.35f,0.35f));
	
	// 死亡処理
	if (m_life <= 0 || GetSize().x <= 0.0f || GetSize().y <= 0.0f)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CCrushEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 生成
//-----------------------------------------
CCrushEffect* CCrushEffect::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3 inMove, const D3DXCOLOR inColor, const CBlock::BLOCK_TYPE inTeam)
{
	CCrushEffect* crush = new CCrushEffect;

	if (crush == nullptr)
	{
		return nullptr;
	}

	crush->Init();
	D3DXVECTOR3 pos = inPos;
	pos.x += ((rand() / (float)RAND_MAX) * (25.0f - -25.0f)) + -25.0f;
	pos.y += ((rand() / (float)RAND_MAX) * (25.0f - -25.0f)) + -25.0f;
	crush->SetPos(inPos);
	crush->SetColor(inColor);
	crush->m_team = inTeam;
	crush->SetSize(D3DXVECTOR2(15.0f, 15.0f));
	crush->m_move.x = inMove.x + ((rand() / (float)RAND_MAX) * (1.0f - -1.0f)) + -1.0f;
	crush->m_move.y = inMove.y + ((rand() / (float)RAND_MAX) * (1.0f - -1.0f)) + -1.0f;
	crush->m_move.z = 0.0f;
	D3DXVec3Normalize(&crush->m_move, &crush->m_move);
	crush->m_move *= 5.0f;

	return crush;
}
