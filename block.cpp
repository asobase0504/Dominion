//==================================================
//
// �����`�̃|���S���̐���
// Author : Yuda Kaito
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "block.h"
#include "character.h"
#include "bullet.h"
#include "application.h"
#include "block_scraped.h"
#include "block_color_addition.h"

// �f�o�b�O
#include <assert.h>

const int CBlock::PRIORITY_BELONG = 1;

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG),
	m_team(CBlock::BLOCK_TYPE::NONE),
	m_scraped(nullptr),
	m_colorAddition(nullptr)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CBlock::~CBlock()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
HRESULT CBlock::Init()
{
	CObject2D::Init();
	SetTexture("BLOCK");
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CBlock::Uninit()
{
	if (m_colorAddition != nullptr)
	{
		m_colorAddition->Uninit();
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

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CBlock::Update()
{
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

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CBlock::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
CBlock* CBlock::Create(CBlock::BLOCK_TYPE type)
{
	CBlock* block = new CBlock;

	if (block == nullptr)
	{
		return nullptr;
	}

	block->Init();

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

//--------------------------------------------------
// ��ʂ̐؂�ւ�
//--------------------------------------------------
void CBlock::ChangeType(DIRECTION inDirection)
{
	switch (m_team)
	{
	case CBlock::BLOCK_TYPE::NONE:
		break;
	case CBlock::BLOCK_TYPE::BLOCK_01:
		m_team = CBlock::BLOCK_TYPE::BLOCK_02;
		SetColor(CApplication::GetInstance()->GetColor(1));

		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos,CBlock::BLOCK_01, inDirection);
		break;
	case CBlock::BLOCK_TYPE::BLOCK_02:
		m_team = CBlock::BLOCK_TYPE::BLOCK_01;
		SetColor(CApplication::GetInstance()->GetColor(0));
		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos, CBlock::BLOCK_02, inDirection);
		break;
	default:
		MessageBox(NULL, TEXT("�z��O�̗񋓌^�����o�B"), TEXT("swith���̏�����"), MB_ICONHAND);
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ��ʂ̎擾
//--------------------------------------------------
CBlock::BLOCK_TYPE CBlock::GetType()
{
	return m_team;
}

//--------------------------------------------------
// ����Ă�I�u�W�F�N�g���܂�����Ă邩�Č�������
//--------------------------------------------------
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

//--------------------------------------------------
// ����Ă�I�u�W�F�N�g��ݒ肷��
//--------------------------------------------------
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

void CBlock::SetAdditionColor()
{
	if (m_colorAddition != nullptr)
	{
		return;
	}

	m_colorAddition = CBlockColorAddition::Create(m_pos, this);
}
