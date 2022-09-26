//==================================================
//
// �����`�̃|���S���̐���
// Author : Yuda Kaito
//
//==================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "block.h"
#include "crush_effect.h"
#include "character.h"
#include "bullet.h"
#include "application.h"
#include "map.h"
#include "block_scraped.h"
#include "block_color_addition.h"

// �f�o�b�O
#include <assert.h>

const int CBlock::PRIORITY_BELONG = 1;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG),
	m_team(CBlock::BLOCK_TYPE::NONE),
	m_scraped(nullptr),
	m_colorAddition(nullptr),
	m_isDuringPop(true)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CBlock::~CBlock()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
HRESULT CBlock::Init()
{
	CObject2D::Init();
	SetTexture("BLOCK");
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CBlock::Uninit()
{
	if (m_colorAddition != nullptr)
	{
		m_colorAddition = nullptr;
	}

	if (m_scraped != nullptr)
	{
		m_scraped->Uninit();
		delete m_scraped;
		m_scraped = nullptr;
	}

	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CBlock::Update()
{
	UpdateInPop();

	if (m_scraped != nullptr)
	{
		m_scraped->Update();
	}

	if (m_colorAddition != nullptr)
	{
		m_colorAddition->Update();
		if (m_colorAddition->GetIsDeleted())
		{
			m_colorAddition = nullptr;
		}
	}

	ResurveyRidingObject();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CBlock::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------
// �ݒ�
//-----------------------------------------------------------------------------
CBlock* CBlock::Create(CBlock::BLOCK_TYPE type,CMap* inParent)
{
	CBlock* block = new CBlock;

	if (block == nullptr)
	{
		return nullptr;
	}

	block->Init();

	block->m_parent = inParent;
	block->m_team = type;

	switch (type)
	{
	case CBlock::BLOCK_TYPE::NONE:
		block->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		break;
	case CBlock::BLOCK_TYPE::BLOCK_01:
		block->SetColor(CApplication::GetInstance()->GetColor(0));
		break;
	case CBlock::BLOCK_TYPE::BLOCK_02:
		block->SetColor(CApplication::GetInstance()->GetColor(1));
		break;
	default:
		break;
	}

	return block;
}

//-----------------------------------------------------------------------------
// ��ʂ̐؂�ւ�
//-----------------------------------------------------------------------------
void CBlock::ChangeType(DIRECTION inDirection)
{
	D3DXVECTOR3 move(0.0f,0.0f,0.0f);

	switch (inDirection)
	{
	case CBlock::TOP:
		move.y += -1.5f;
		break;
	case CBlock::DOWN:
		move.y += 1.5f;
		break;
	case CBlock::LEFT:
		move.x += -1.5f;
		break;
	case CBlock::RIGHT:
		move.x += 1.5f;
		break;
	default:
		break;
	}

	switch (m_team)
	{
	case CBlock::BLOCK_TYPE::NONE:
		break;
	case CBlock::BLOCK_TYPE::BLOCK_01:
		m_team = CBlock::BLOCK_TYPE::BLOCK_02;
		for (int i = 0; i < 2; i++)
		{
			CCrushEffect::Create(m_pos, move, m_col, m_team, m_parent);
		}

		SetColor(CApplication::GetInstance()->GetColor(1));

		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos,CBlock::BLOCK_01, inDirection, m_parent->GetBlockSize());
		break;
	case CBlock::BLOCK_TYPE::BLOCK_02:
		m_team = CBlock::BLOCK_TYPE::BLOCK_01;

		for (int i = 0; i < 2; i++)
		{
			CCrushEffect::Create(m_pos, move, m_col, m_team, m_parent);
		}

		SetColor(CApplication::GetInstance()->GetColor(0));
		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos, CBlock::BLOCK_02, inDirection, m_parent->GetBlockSize());
		break;
	default:
		MessageBox(NULL, TEXT("�z��O�̗񋓌^�����o�B"), TEXT("swith���̏�����"), MB_ICONHAND);
		assert(false);
		break;
	}
}

//-----------------------------------------------------------------------------
// ��ʂ̎擾
//-----------------------------------------------------------------------------
CBlock::BLOCK_TYPE CBlock::GetType()
{
	return m_team;
}

//-----------------------------------------------------------------------------
// ����Ă�I�u�W�F�N�g���܂�����Ă邩�Č�������
//-----------------------------------------------------------------------------
void CBlock::ResurveyRidingObject()
{
	for (auto it = ridingObject.begin(); it != ridingObject.end();)
	{
		CObject::TYPE type = (*it)->GetType();

		switch (type)
		{
		case CObject::TYPE::CHARACTER:
		{
			CCharacter* character = (CCharacter*)(*it);	// �C�e���[�^���L�����N�^�[�ɕϊ��B

			if (!character->HitWithBlock(this))
			{ // �������ĂȂ��ꍇ
				// ����Ă�I�u�W�F�N�g�𓮓I�z�񂩂�폜
				it = ridingObject.erase(it);
				continue;
			}
		}
		break;
		case CObject::TYPE::BULLET:
		{
			CBullet* bullet = (CBullet*)(*it);	// �C�e���[�^��e�ɕϊ��B

			if (!bullet->HitWithBlock(this))
			{ // �������ĂȂ��ꍇ
			  // ����Ă�I�u�W�F�N�g�𓮓I�z�񂩂�폜
				it = ridingObject.erase(it);
				continue;
			}
		}
			break;
		default:
			break;
		}

		// �폜���Ȃ��ꍇ
		it++;
	}
}

//-----------------------------------------------------------------------------
// ���Z���Ă���u���b�N�̍폜�˗�
//-----------------------------------------------------------------------------
void CBlock::ColorAdditionDelete()
{
	if (m_colorAddition != nullptr)
	{
		m_colorAddition->SetIsDeleted(true);
	}
}

//-----------------------------------------------------------------------------
// ����Ă���u���b�N�̍폜�˗�
//-----------------------------------------------------------------------------
void CBlock::ScrapedDelete()
{
	if (m_scraped != nullptr)
	{
		m_scraped->SetIsDeleted();
	}
}

//-----------------------------------------------------------------------------
// �\��ʒu��ݒ肷��
//-----------------------------------------------------------------------------
void CBlock::SetPosPlan(const D3DXVECTOR3& inPos)
{
	m_posPlan = inPos;

	CApplication* application = CApplication::GetInstance();
	D3DXVECTOR3 pos;
	pos.x = (rand() / (float)RAND_MAX) * (application->SCREEN_WIDTH);
	pos.y = (rand() / (float)RAND_MAX) * (application->SCREEN_HEIGHT);
	pos.z = 0.0f;

	SetPos(pos);
}

//-----------------------------------------------------------------------------
// �\��T�C�Y��ݒ肷��
//-----------------------------------------------------------------------------
void CBlock::SetSizePlan(const D3DXVECTOR2& inSize)
{
	m_sizePlan = inSize;

	float size = ((rand() / (float)RAND_MAX) * (5.0f - 0.0f)) + 0.0f;

	SetSize(D3DXVECTOR2(size, size));
}

//-----------------------------------------------------------------------------
// ����Ă�I�u�W�F�N�g��ݒ肷��
//-----------------------------------------------------------------------------
void CBlock::SetRidingObject(CObject * inObject)
{
	for (const auto& it : ridingObject)
	{
		// �����I�u�W�F�N�g���������ꍇ�͒e���B
		if (it == inObject)
		{
			return;
		}
	}
	// ����Ă�I�u�W�F�N�g��ۑ�����
	ridingObject.push_back(inObject);
}

//-----------------------------------------------------------------------------
// �F�����Z������u���b�N��ݒ肷��
//-----------------------------------------------------------------------------
void CBlock::SetAdditionColor()
{
	if (m_colorAddition != nullptr)
	{
		return;
	}

	m_colorAddition = CBlockColorAddition::Create(m_pos, this, m_parent);
}

//-----------------------------------------------------------------------------
// �o�����̍X�V
//-----------------------------------------------------------------------------
void CBlock::UpdateInPop()
{
	if (!m_isDuringPop)
	{
		return;
	}

	bool isWithPos = false;
	{
		D3DXVECTOR3 dist = m_posPlan - GetPos();
		float length = D3DXVec3Length(&dist);

		if (!(length < 1.0f && length > -1.0f))
		{
			D3DXVec3Normalize(&dist, &dist);

			D3DXVECTOR3 move = dist;
			move *= length * 0.1f;
			SetPos(GetPos() + move);
		}
		else
		{
			isWithPos = true;
			SetPos(m_posPlan);
		}
	}

	bool isWithSize = false;
	{
		D3DXVECTOR2 dist = m_sizePlan - GetSize();
		float length = D3DXVec2Length(&dist);

		if (!(length < 1.0f && length > -1.0f))
		{
			D3DXVec2Normalize(&dist, &dist);

			D3DXVECTOR2 move = dist;
			move *= length * 0.1f;
			SetSize(GetSize() + move);
		}
		else
		{
			isWithSize = true;
			SetSize(m_sizePlan);
		}
	}

	if (isWithPos && isWithSize)
	{
		m_isDuringPop = false;
	}
}
