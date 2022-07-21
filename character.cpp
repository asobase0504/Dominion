//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "character.h"
#include "application.h"
#include "input_keybord.h"
#include "bullet.h"
#include "controller.h"

#include "collision.h"
#include "block.h"
#include <assert.h>

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CCharacter::CCharacter(CObject::TYPE type) :
	CObject2D(type),
	m_controller(nullptr)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CCharacter::~CCharacter()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CCharacter::Uninit()
{
	CObject2D::Uninit();

}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CCharacter::Update()
{
	// �ړ�
	Move();

	// �e�̔���
	BulletShot();

	// �X�N���[���O�ɏo����
	ScreenFromOutTime();

	// �u���b�N�Ƃ̓����蔻��
	Collision();
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CCharacter::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// �ړ�
//-----------------------------------------
void CCharacter::Move()
{
	if (m_controller == nullptr)
	{
		assert(false);
		return;
	}

	// �����x�N�g���|����ړ���
	m_move = m_controller->Move() * 2.0f * 2.0f;
	m_pos += m_move;
	CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
}

//-----------------------------------------
// �e�̔���
//-----------------------------------------
void CCharacter::BulletShot()
{
	if (m_controller == nullptr)
	{
		assert(false);
		return;
	}

	// �e�̔���
	switch (m_controller->BulletShot())
	{
	case CController::UP_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -10.0f, 0.0f),m_team);
		break;
	case CController::DOWN_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_team);
		break;
	case CController::LEFT_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), m_team);
		break;
	case CController::RIGHT_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(10.0f, 0.0f, 0.0f), m_team);
		break;

	default:
		break;
	}
}

//-----------------------------------------
// �`�[���̐ݒ�
//-----------------------------------------
void CCharacter::SetTeam(const TEAM inTeam)
{
	m_team = inTeam;

	switch (m_team)
	{
	case TEAM_00:
		CObject2D::SetCol(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));		// �ʒu�̐ݒ�
		break;
	case TEAM_01:
		CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �ʒu�̐ݒ�
		break;
	default:
		break;
	}
}

//-----------------------------------------
// ����
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

	return player;
}

//-----------------------------------------
// �X�N���[���O�ɏo����
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
// �����蔻��
//-----------------------------------------
void CCharacter::Collision()
{
	for (auto it = GetMyObject()->begin(); it != GetMyObject()->end(); it++)
	{
		if ((*it)->GetIsDeleted())
		{
			continue;
		}

		if ((*it)->CObject::GetType() == CObject::TYPE::BLOCK)
		{
			HitWithBlock((CBlock*)(*it));
		}

		if ((*it)->CObject::GetType() == CObject::TYPE::BULLET)
		{
			HitWithBullet((CBullet*)(*it));
		}
	}
}

//-----------------------------------------
// �u���b�N�Ƃ̓����蔻��
//-----------------------------------------
void CCharacter::HitWithBlock(CBlock* inBlock)
{
	CBlock* block = inBlock;

	int blockType = (int)block->CBlock::GetType();

	if ((int)m_team == blockType)
	{
		return;
	}

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float dist;

	if (m_move.y > 0.0f)
	{
		// �v���C���[��A�u���b�N���̓����蔻��
		if (Collision::RectangleTop(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.y += 1.0f;
			dist = (size.y) * 0.5f + (m_pos.y - outpos.y);
			m_pos.y -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
		}
	}
	if (m_move.x > 0.0f)
	{
		if (Collision::RectangleLeft(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.x += 1.0f;
			dist = (size.x) * 0.5f + (m_pos.x - outpos.x);
			m_pos.x -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
		}
	}
	if (m_move.x < 0.0f)
	{
		if (Collision::RectangleRight(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.x += -1.0f;
			float dist = (-size.x) * 0.5f + (m_pos.x - outpos.x);
			m_pos.x -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
		}
	}
	if (m_move.y < 0.0f)
	{
		if (Collision::RectangleDown(block->GetPos(), D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			vec.y = -1.0f;
			dist = (-size.y) * 0.5f + (m_pos.y - outpos.y);

			m_pos.y -= dist + dist * 0.0001f;
			CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
		}
	}
}

//-----------------------------------------
// �e�Ƃ̓����蔻��
//-----------------------------------------
void CCharacter::HitWithBullet(CBullet* inBullet)
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
	if (Collision::RectangleTop(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleLeft(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleRight(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL)
		|| Collision::RectangleDown(pBullet->GetPos(), bulletSize, m_pos, mySize, &outpos, NULL, NULL))
	{
		m_isDeleted = true;
	}
}
