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
#include "bullet_effect.h"
#include "application.h"
#include "collision.h"
#include "block.h"
#include "stage.h"
#include "map.h"
#include "sound.h"

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

	CBulletEffect* effect = CBulletEffect::Create(GetPos(),m_map->GetBlockSize());
	effect->SetColor(GetColor());

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
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
		break;
	case CCharacter::TEAM_01:
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
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
CBullet* CBullet::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove, const CCharacter::TEAM inTeam,CMap* inMap)
{
	CBullet* bullet = new CBullet;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->m_map = inMap;
	float size = bullet->m_map->GetBlockSize();		// �傫���擾
	bullet->SetPos(inPos);
	bullet->m_move = inMove;

	D3DXVec3Normalize(&bullet->m_move, &bullet->m_move);

	bullet->m_move *= size * 0.25f;

	bullet->SetTeam(inTeam);
	bullet->SetSize(D3DXVECTOR2(size * 0.25f, size * 0.25f));
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SHOT);

	return bullet;
}

//-----------------------------------------
// �S�̂̓����蔻��
//-----------------------------------------
void CBullet::Collision()
{
	// ����Ă�u���b�N�����Z�b�g����
	m_ofBlockCount = 0;

	// ���w�̃u���b�N�ɓ��������ꍇ���̃u���b�N�̔ԍ���ۑ����郉���_���֐�
	auto HitBlock = [this](int x, int y)
	{

		// �����������ۂ�
		bool isHit = HitWithBlock(m_map->GetBlock(x, y));

		if (isHit)
		{ // ���������ꍇ
			if (SetBlockIndex(m_ofBlockCount, { x, y }))
			{ // �ݒ�o�����ꍇ
				m_ofBlockCount++;
			}
		}
	};

	for (int i = 0; i < (int)m_ofBlockIndex.size(); i++)
	{
		if (m_ofBlockIndex[i].empty())
		{
			continue;
		}

		int CenterX = m_ofBlockIndex[i][0];		// X���̎��ےl
		int LeftX = m_ofBlockIndex[i][0] - 1;	// X���̍���
		int RightX = m_ofBlockIndex[i][0] + 1;	// X���̉E��

		int CenterY = m_ofBlockIndex[i][1];		// Y���̎��ےl
		int TopY = m_ofBlockIndex[i][1] - 1;	// Y���̏㑤
		int BottomY = m_ofBlockIndex[i][1] + 1;	// Y���̉���

		// �u���b�N�[�̏ꍇ�̏���
		if (LeftX < 0)
		{
			LeftX = m_map->GetMaxXBlock() - 1;
		}
		if (RightX > m_map->GetMaxXBlock() - 1)
		{
			RightX = 0;
		}
		if (TopY < 0)
		{
			TopY = m_map->GetMaxYBlock() - 1;
		}
		if (BottomY > m_map->GetMaxYBlock() - 1)
		{
			BottomY = 0;
		}

		// �u���b�N�̓����蔻��
		HitBlock(LeftX, TopY);		// ����
		HitBlock(CenterX, TopY);	// ��
		HitBlock(RightX, TopY);		// �E��
		HitBlock(LeftX, CenterY);	// ���^��
		HitBlock(CenterX, CenterY);	// �^��
		HitBlock(RightX, CenterY);	// �E�^��
		HitBlock(LeftX, BottomY);	// ����
		HitBlock(CenterX, BottomY);	// ��
		HitBlock(RightX, BottomY);	// �E��
	}

	// �I�u�W�F�N�g�S�̂��擾
	for (auto it = GetMyObject(3)->begin(); it != GetMyObject(3)->end(); it++)
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
// �X�N���[���O�ɏo����
//-----------------------------------------
void CBullet::ScreenFromOutTime()
{
	if (m_pos.x + m_size.x <= 0.0f - m_size.x)
	{
		m_pos.x = static_cast<float>(CApplication::GetInstance()->SCREEN_WIDTH);
		SetPos(m_pos);
	}
	if (m_pos.x - m_size.x >= CApplication::GetInstance()->SCREEN_WIDTH + m_size.x)
	{
		m_pos.x = 0.0f;
		SetPos(m_pos);
	}
	if (m_pos.y + m_size.y <= 0.0f - m_size.y)
	{
		m_pos.y = static_cast<float>(CApplication::GetInstance()->SCREEN_HEIGHT);
		SetPos(m_pos);
	}
	if (m_pos.y - m_size.y >= CApplication::GetInstance()->SCREEN_HEIGHT + m_size.y)
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
	D3DXVECTOR3 mySize = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f;

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
		if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(m_size.x, m_size.y, 0.0f), block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f))
		{
			return true;
		}
		return false;
	}

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 pairSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;
	D3DXVECTOR3 mySize = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f;

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

		if (m_move.x < 0.0f)
		{
			block->ChangeType(CBlock::DIRECTION::LEFT);
		}
		else if (m_move.x > 0.0f)
		{
			block->ChangeType(CBlock::DIRECTION::RIGHT);
		}
		else if (m_move.y < 0.0f)
		{
			block->ChangeType(CBlock::DIRECTION::DOWN);
		}
		else if (m_move.y > 0.0f)
		{
			block->ChangeType(CBlock::DIRECTION::TOP);
		}

		return true;
	}
	return false;
}
