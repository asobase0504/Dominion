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
// ��`
//-----------------------------------------
const int CCharacter::LIMIT_BULLET_COUNT = 5;	// �e�̍ő吔
const int CCharacter::RELOAD_TIME = 60;			// �ꔭ�̒e���񕜂��鎞��
const float CCharacter::MOVE_SPEAD = 5.0f;		// �ړ����x

//-----------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//-----------------------------------------
CCharacter::~CCharacter()
{
	assert(m_remainsBulletDisplay.empty());
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	m_remainsBulletCount = LIMIT_BULLET_COUNT;
	m_ofBlockIndex.resize(4);
	SetTexture("PLAYER");	// �e�N�X�`���̎w��
	return S_OK;
}

//-----------------------------------------
// �I��
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
// �X�V
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
	// �ړ�
	Move();

	// �e�̔���
	BulletShot();

	// �e���̉�
	BulletReload();

	// �����蔻��
	Collision();

	m_pos += m_move;
	CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�

	{ // ���g���������Ă���u���b�N�ɋO�Ղ�\��

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		pBlock->SetAdditionColor();
	}

	/*
	// �o�O�̉����̂��ߖ������ɕύX�B���Ԃ��������炱������������Ă��������B
	*/
	// �X�N���[���O�ɏo����
	//ScreenFromOutTime();
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

	// �e���������������_��
	auto Shot = [this](const D3DXVECTOR3& inMove)
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);

		CBullet* bullet = CBullet::Create(pBlock->GetPos(), inMove, m_team, pMap);
		bullet->SetBlockIndex(0, m_ofBlockIndexCenter);
		m_remainsBulletDisplay[m_remainsBulletCount - 1]->SetColorAlpha(0.0f);
		m_remainsBulletCount--;
	};

	// �e�̔���
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

		// �e���o��������u���b�N
		CBlock* pBlock = pMap->GetBlock(m_ofBlockIndexCenter[0], m_ofBlockIndexCenter[1]);
		CBlock* pBlockUp = pMap->GetBlock(m_ofBlockIndexCenter[0] - 1, m_ofBlockIndexCenter[1]);
		CBlock* pBlockDown = pMap->GetBlock(m_ofBlockIndexCenter[0] + 1, m_ofBlockIndexCenter[1]);

		D3DXVECTOR3 move(0.0f, 1.0f, 0.0f);	// �ړ���

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
// �R���g���[���[�̐ݒ�
//-----------------------------------------
void CCharacter::SetController(CController * inOperate)
{
	m_controller = inOperate;
	m_controller->SetToOrder(this);
	m_isOperationState = true;
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
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
		break;
	case TEAM_01:
		CObject2D::SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
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
// �X�e�[�W�̐ݒ�
//-----------------------------------------
void CCharacter::SetStage(CStage * inStage)
{
	m_stage = inStage;
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

		// �񕜂����e���c�e���ɕ\�L����
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
		character->SetSize(m_size);		// �傫���̐ݒ�

		// �u���b�N�̔ԍ��o�^
		for (int i = 0; i < (int)m_ofBlockIndex.size();i++)
		{
			character->SetBlockIndex(i, m_ofBlockIndex[i]);
		}

		// ���S�̈ʒu���ǂ̃u���b�N�ɏ������Ă��邩�`�F�b�N����
		for (int i = 0; i < (int)character->m_ofBlockIndex.size(); i++)
		{
			if (character->m_ofBlockIndex[i].empty())
			{
				continue;
			}

			int x = character->m_ofBlockIndex[i][0];
			int y = character->m_ofBlockIndex[i][1];

			// ���̃u���b�N���L�����N�^�[�̒��S���������Ă�u���b�N���`�F�b�N
	
			CBlock* block = m_stage->GetMap()->GetBlock(x, y);
			D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f);	// �u���b�N�̑傫��

			if (Collision::RectangleAndRectangle(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
			{
				character->m_ofBlockIndexCenter = { x, y };
			}
		}

		character->SetController(m_controller);	// ���ߎ҂̐ݒ�
		isCopied = true;			// �R�s�[�ς݂ɂ���
		character->isCopied = true;	// �R�s�[����R�s�[�ς݂ɂ���
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

	float dist = 0.00001f;	// ������ꏊ�Əo���ʒu�����炷���߂̒l
	if (m_pos.x + m_size.x * 0.5f + dist <= 0.0f ||
		m_pos.y + m_size.y * 0.5f + dist <= 0.0f ||
		m_pos.x - m_size.x * 0.5f - dist >= CApplication::GetInstance()->SCREEN_WIDTH ||
		m_pos.y - m_size.y * 0.5f - dist >= CApplication::GetInstance()->SCREEN_HEIGHT)
	{
		// ���g�̍폜
		m_isDeleted = true;
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEAD);
	}
}

//-----------------------------------------
// �����蔻��
//-----------------------------------------
void CCharacter::Collision()
{
	for (int i = 0; i < (int)m_ofBlockIndex.size(); i++)
	{
		m_ofBlockIndex[i].clear();
	}

	m_ofBlockCount = 0;

	// ��̃u���b�N�Ƃ̓����蔻�菈��
	auto HitBlock = [this](int x, int y, DIRECTION inDirection, std::vector<DIRECTION> inAround)
	{

		CMap* pMap = m_stage->GetMap();
		CBlock* block = pMap->GetBlock(x, y);

		if ((int)m_team == (int)block->CBlock::GetType())
		{
			/* �������`�[���̃u���b�N�������火 */

			if (!HitWithBlock(block))
			{
				return;
			}

			/* ���u���b�N�ɐڐG���Ă����火 */

			pMap->GetBlock(x, y)->SetRidingObject(this);	// �u���b�N���Ɏ��g��ۑ�����
			SetBlockIndex(m_ofBlockCount, { x, y });

			// ���̃u���b�N���L�����N�^�[�̒��S���������Ă�u���b�N���`�F�b�N
			block = pMap->GetBlock(x, y);
			D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f);	// �u���b�N�̑傫��

			D3DXVECTOR3 movePlanPos = m_pos + m_move;

			if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), block->GetPos(), blockSize))
			{
				/* ���L�����N�^�[�̒��S���������Ă����ꍇ�� */
				m_ofBlockIndexCenter = { x, y };
			}
		}
		else
		{
			/* ���ʃ`�[���̃u���b�N�������火 */

			HitWithAnotherTeamBlock(block, inDirection, inAround);
		}
	};

	{ // �������Ă��Ȃ��u���b�N�Ƃ̓����蔻��
		int CenterX = m_ofBlockIndexCenter[0];
		int LeftX = m_ofBlockIndexCenter[0] - 1;
		int RightX = m_ofBlockIndexCenter[0] + 1;

		int CenterY = m_ofBlockIndexCenter[1];
		int TopY = m_ofBlockIndexCenter[1] - 1;
		int BottomY = m_ofBlockIndexCenter[1] + 1;


		CMap* pMap = m_stage->GetMap();

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

		HitBlock(CenterX, TopY, CenterTop, inAround);		// ��
		HitBlock(CenterX, BottomY, CenterBottom, inAround);	// ��
		HitBlock(LeftX, CenterY, LeftCenter, inAround);		// ���^��
		HitBlock(CenterX, CenterY, CenterCenter, inAround);		// �^��
		HitBlock(RightX, CenterY, RightCenter, inAround);	// �E�^��
		HitBlock(LeftX, TopY, LeftTop, inAround);			// ����
		HitBlock(RightX, TopY, RightTop, inAround);			// �E��
		HitBlock(LeftX, BottomY, LeftBottom, inAround);		// ����
		HitBlock(RightX, BottomY, RightBottom, inAround);	// �E��

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

			m_isDeleted = true;	// ���S��Ԃɂ���
			CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEAD);

		}
	}
}

//-----------------------------------------
// �u���b�N�Ƃ̓����蔻��
//-----------------------------------------
bool CCharacter::HitWithBlock(CBlock* inBlock)
{
	D3DXVECTOR3 movePlanPos = m_pos + m_move;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(inBlock->GetSize().x, inBlock->GetSize().y, 0.0f);	// �u���b�N�̑傫��

	if (Collision::RectangleAndRectangle(movePlanPos, D3DXVECTOR3(m_size.x, m_size.y, 0.0f), inBlock->GetPos(), blockSize))
	{
		return true;
	}
	return false;
}

//-----------------------------------------
// �ʃ`�[���̃u���b�N�Ƃ̓����蔻��
//-----------------------------------------
void CCharacter::HitWithAnotherTeamBlock(CBlock * inBlock, DIRECTION inDirection ,std::vector<DIRECTION> inAround)
{
	CBlock* block = inBlock;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(block->GetSize().x, block->GetSize().y, 0.0f) * 0.5f;	// �u���b�N�̑傫��

	std::function<bool(Collision::RECTANGLE_DIRECTION)> Collision;

	Collision = [this, block, blockSize, inDirection, inAround](Collision::RECTANGLE_DIRECTION inDirect)->bool
	{
		D3DXVECTOR3 movePlanPos = m_pos + m_move;
		D3DXVECTOR3 outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �v���C���[��A�u���b�N���̓����蔻��
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
	case LeftTop:	// ����
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
	case CenterTop:	// ��
		Collision(Collision::RECTANGLE_DIRECTION::DOWN);
		break;
	case RightTop:	// �E��
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
	case LeftCenter:	// ��
		Collision(Collision::RECTANGLE_DIRECTION::RIGHT);
		break;
	case CenterCenter:	// �^��
		break;
	case RightCenter:	// �E
		Collision(Collision::RECTANGLE_DIRECTION::LEFT);
		break;
	case LeftBottom:	// ����
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
	case CenterBottom:	// ��
		Collision(Collision::RECTANGLE_DIRECTION::TOP);
		break;
	case RightBottom:	// �E��
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
