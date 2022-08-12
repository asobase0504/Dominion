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
// ��`
//-----------------------------------------
const int CCharacter::LIMIT_BULLET_COUNT = 5;	// �e�̍ő吔
const int CCharacter::RELOAD_TIME = 100;		// �ꔭ�̒e���񕜂��鎞��
const float CCharacter::MOVE_SPEAD = 4.0f;		// �ړ����x

//-----------------------------------------
// �R���X�g���N�^
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
	m_remainsBulletCount = LIMIT_BULLET_COUNT;
	m_ofBlockIndex.resize(4);
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);	// �e�N�X�`���̎w��
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

	// �e���̉�
	BulletReload();

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
	m_move = m_controller->Move() * MOVE_SPEAD;
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

	// �c�e����0��������B
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

	// �e�̔���
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
// �R���g���[���[�̐ݒ�
//-----------------------------------------
void CCharacter::SetController(CController * inOperate)
{
	m_controller = inOperate;
	m_controller->SetToOrder(this);
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
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F�̐ݒ�
		break;
	case TEAM_01:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�
		break;
	default:
		break;
	}
}

//-----------------------------------------
// ����Ă�u���b�N�̔ԍ���ݒ�
//-----------------------------------------
bool CCharacter::SetBlockIndex(const int count, std::vector<int> inIndex)
{
	// �󂾂����ꍇ
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

	// �ǉ�
	m_ofBlockIndex[count] = inIndex;
	m_ofBlockCount++;
	return true;
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
	float rot = D3DX_PI * 2.0f / LIMIT_BULLET_COUNT;
	for (int i = 0; i < LIMIT_BULLET_COUNT; i++)
	{
		player->m_remainsBulletDisplay.push_back(CRemaubsBullet::Create(player, rot * i));
	}
	return player;
}

//-----------------------------------------
// �e�̃����[�h����
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
// �X�N���[���O�ɏo����
//-----------------------------------------
void CCharacter::ScreenFromOutTime()
{
	// ���g�̕���
	auto Copy = [this](D3DXVECTOR3 inPos)
	{
		if (isCopied)
		{
			return;
		}

		CCharacter* character = Create(m_team);
		character->SetPos(inPos);
		character->SetSize(D3DXVECTOR2(25.0f, 25.0f));		// �傫���̐ݒ�

		// �u���b�N�̔ԍ��o�^
		for (int i = 0; i < m_ofBlockIndex.size();i++)
		{
			character->SetBlockIndex(i, m_ofBlockIndex[i]);
		}

		character->SetController(m_controller);	// ���ߎ҂̐ݒ�
		character->isCopied = true;	// �R�s�[�ς݂ɂ���
		isCopied = true;	// �R�s�[�ς݂ɂ���
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

	float dist = 0.001f;	// ������ꏊ�Əo���ʒu�����炷���߂̒l
	if (m_pos.x + size.x + dist <= 0.0f - size.x ||
		m_pos.y + size.y + dist <= 0.0f - size.y ||
		m_pos.x - size.x - dist >= CApplication::GetInstance()->SCREEN_WIDTH + size.x ||
		m_pos.y - size.y - dist >= CApplication::GetInstance()->SCREEN_HEIGHT + size.y)
	{
		// ���g�̍폜
		m_isDeleted = true;
	}
}

//-----------------------------------------
// �����蔻��
//-----------------------------------------
void CCharacter::Collision()
{
	m_ofBlockCount = 0;

	// ��̃u���b�N�Ƃ̓����蔻�菈��
	auto HitBlock = [this](int x, int y)
	{
		CGame* game = (CGame*)CApplication::GetInstance()->GetMode();
		CMap* pMap = game->GetMap();

		bool isHit = HitWithBlock(pMap->GetBlock(x, y));	// �����������ۂ�

		if (!isHit)
		{ // �������ĂȂ��ꍇ
			return;
		}

		/* �����������ꍇ�� */
		pMap->GetBlock(x, y)->SetRidingObject(this);	// �u���b�N���Ɏ��g��ۑ�����
		SetBlockIndex(m_ofBlockCount, { x, y });

		// ���̃u���b�N���L�����N�^�[�̒��S���������Ă�u���b�N���`�F�b�N
		CBlock* block = pMap->GetBlock(x, y);
		D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// �u���b�N�̑傫��

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

		// �u���b�N�[�̏ꍇ�̏���
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
// �u���b�N�Ƃ̓����蔻��
//-----------------------------------------
bool CCharacter::HitWithBlock(CBlock* inBlock)
{
	CBlock* block = inBlock;

	int blockType = (int)block->CBlock::GetType();
	D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// �u���b�N�̑傫��

	// �����Ɠ��������������ꍇ
	if ((int)m_team == blockType)
	{
		if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(size.x, size.y, 0.0f), block->GetPos(), blockSize))
		{
			return true;
		}
		return false;
	}

	/* �������ƈႤ�����������ꍇ�� */

	D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float dist;

	if (m_move.y > 0.0f)
	{
		// �v���C���[��A�u���b�N���̓����蔻��
		if (Collision::RectangleTop(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (size.y) * 0.5f + (m_pos.y - outpos.y);	// ����
			m_pos.y -= dist + dist * 0.0001f;	// �ʒu�̐ݒ�
			CObject2D::SetPos(m_pos);		// �ʒu�̔��f
		}
	}
	if (m_move.x > 0.0f)
	{
		// �v���C���[�E�A�u���b�N���̓����蔻��
		if (Collision::RectangleLeft(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (size.x) * 0.5f + (m_pos.x - outpos.x);	// ����
			m_pos.x -= dist + dist * 0.0001f;	// �ʒu�̐ݒ�
			CObject2D::SetPos(m_pos);		// �ʒu�̔��f
		}
	}
	if (m_move.x < 0.0f)
	{
		// �v���C���[���A�u���b�N�E�̓����蔻��
		if (Collision::RectangleRight(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (-size.x) * 0.5f + (m_pos.x - outpos.x);	// ����
			m_pos.x -= dist + dist * 0.0001f;	// �ʒu�̐ݒ�
			CObject2D::SetPos(m_pos);		// �ʒu�̔��f
		}
	}
	if (m_move.y < 0.0f)
	{
		// �v���C���[���A�u���b�N��̓����蔻��
		if (Collision::RectangleDown(block->GetPos(), blockSize, m_pos, D3DXVECTOR3(size.x, size.y, 0.0f) * 0.5f, &outpos, NULL, NULL))
		{
			dist = (-size.y) * 0.5f + (m_pos.y - outpos.y);	// ����
			m_pos.y -= dist + dist * 0.0001f;	// �ʒu�̐ݒ�
			CObject2D::SetPos(m_pos);		// �ʒu�̔��f
		}
	}
	return false;
}

//-----------------------------------------
// �e�Ƃ̓����蔻��
//-----------------------------------------
void CCharacter::HitWithBullet(CBullet* inBullet)
{
	CBullet* pBullet = inBullet;

	// ���������Ȃ�e��
	if ((int)m_team == (int)pBullet->CBullet::GetTeam())
	{
		return;
	}

	std::vector<std::vector<int>> bulletOfBlock = pBullet->GetOfBlock();

	// ����Ă�u���b�N�̐�����
	for (int i = 0; i < m_ofBlockIndex.size(); i++)
	{
		if (m_ofBlockIndex[i].empty())
		{
			continue;
		}

		/* ���v���C���[�̏����u���b�N����ł͂Ȃ������ꍇ�� */

		for (int j = 0; j < bulletOfBlock.size(); j++)
		{
			if (bulletOfBlock[j].empty())
			{
				continue;
			}

			/* ���e�̏����u���b�N����ł͂Ȃ������ꍇ�� */

			if (m_ofBlockIndex[i] == bulletOfBlock[j])
			{
				m_isDeleted = true;
				return;
			}
		}
	}
}
