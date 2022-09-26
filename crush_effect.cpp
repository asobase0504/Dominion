//=========================================
// 
// �ӂ���G�t�F�N�g
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "crush_effect.h"
#include "application.h"
#include "stage.h"
#include "map.h"
#include "block.h"
#include "collision.h"

//-----------------------------------------
// ��`
//-----------------------------------------

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CCrushEffect::CCrushEffect(CObject::TYPE type) :
	CObject2D(type, 2),
	m_life(0)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CCrushEffect::~CCrushEffect()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CCrushEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// �����l
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CCrushEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CCrushEffect::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// �̗͂̌���
	m_life--;

	int Xindex = m_map->GetMaxXBlock();
	int Yindex = m_map->GetMaxYBlock();

	for (int x = 0; x < Xindex ; x++)
	{
		for (int y = 0; y < Yindex; y++)
		{
			CBlock* block = m_map->GetBlock(x, y);

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
	
	// ���S����
	if (m_life <= 0 || GetSize().x <= 0.0f || GetSize().y <= 0.0f)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CCrushEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// ����
//-----------------------------------------
CCrushEffect* CCrushEffect::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3 inMove, const D3DXCOLOR inColor, const CBlock::BLOCK_TYPE inTeam,CMap* inMap)
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
	crush->m_map = inMap;
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
