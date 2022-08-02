//=========================================
// 
// �I�u�W�F�N�g�N���X
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
// �R���X�g���N�^
//-----------------------------------------
CBullet::CBullet(CObject::TYPE type) :
	CObject2D(type),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_life(0)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CBullet::~CBullet()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CBullet::Init()
{
	CObject2D::Init();
	m_ofBlockIndex.resize(4);
	m_life = 200;	// �����l
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CBullet::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// �̗͂̌���
	m_life--;

	// �ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	// �����蔻��
	Collision();

	// �X�N���[���O�̏���
	ScreenFromOutTime();

	// ���S����
	if (m_life <= 0)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// �`�[���̐ݒ�
//-----------------------------------------
void CBullet::SetTeam(const CCharacter::TEAM inTeam)
{
	m_team = inTeam;

	switch (m_team)
	{
	case CCharacter::TEAM_00:
		CObject2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �ʒu�̐ݒ�
		break;
	case CCharacter::TEAM_01:
		CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �ʒu�̐ݒ�
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
// ����
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
// �S�̂̓����蔻��
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

		// �u���b�N�[�̏ꍇ�̏���
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

		// �u���b�N�̓����蔻��
		SetHitBlock(LeftX, TopY);			// ����
		SetHitBlock(CenterX, TopY);			// ��
		SetHitBlock(RightX, TopY);			// �E��
		SetHitBlock(LeftX, CenterY);		// ���^��
		SetHitBlock(CenterX, CenterY);		// �^��
		SetHitBlock(RightX, CenterY);		// �E�^��
		SetHitBlock(LeftX, BottomY);		// ����
		SetHitBlock(CenterX, BottomY);		// ��
		SetHitBlock(RightX, BottomY);		// �E��
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
// �u���b�N�Ƃ̓����蔻��Ăяo���ƁA����Ă�u���b�N�̐ݒ�
//------------------------------------------------------------------
void CBullet::SetHitBlock(int x, int y)
{
	CMap* pMap = CApplication::GetInstance()->GetGame()->GetMap();

	bool isHit = HitWithBlock(pMap->GetBlock(x, y));	// �����������ۂ�

	if (isHit)
	{ // ���������ꍇ
		if (SetBlockIndex(m_ofBlockCount, { x, y }))
		{ // �ݒ�o�����ꍇ
			m_ofBlockCount++;
		}
	}
}

//-----------------------------------------
// �X�N���[���O�ɏo����
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
// �e�Ƃ̓����蔻��
//-----------------------------------------
void CBullet::HitWithBullet(CBullet* inBullet)
{
	CBullet* pBullet = inBullet;

	if ((int)m_team == (int)pBullet->CBullet::GetTeam())
	{
		return;
	}

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�̃T�C�Y
	D3DXVECTOR3 bulletSize = D3DXVECTOR3(pBullet->GetSize().x, pBullet->GetSize().y, 0.0f) * 0.5f;

	// �����̃T�C�Y
	D3DXVECTOR3 mySize = D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f;

	// �v���C���[��A�u���b�N���̓����蔻��
	if (Collision::CircleAndCircle(pBullet->GetPos(), D3DXVec3Length(&bulletSize), m_pos, D3DXVec3Length(&mySize)))
	{
		inBullet->m_isDeleted = true;
		m_isDeleted = true;
	}
}

//-----------------------------------------
// �u���b�N�Ƃ̓����蔻��
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
