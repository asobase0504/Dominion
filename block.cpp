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
// �f�o�b�O
#include <assert.h>

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type),
	m_team(BLOCK_TYPE::NONE)
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
	SetTexture(CTexture::TEXTURE::TEXTURE_BLOCK);
	return S_OK;
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
void CBlock::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CBlock::Update()
{
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
	case BLOCK_TYPE::NONE:
		block->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	case BLOCK_TYPE::BLOCK_01:
		block->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		block->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	default:
		break;
	}

	return block;
}

//--------------------------------------------------
// ��ʂ̐؂�ւ�
//--------------------------------------------------
void CBlock::ChangeType()
{
	switch (m_team)
	{
	case BLOCK_TYPE::NONE:
		break;
	case BLOCK_TYPE::BLOCK_01:
		m_team = BLOCK_TYPE::BLOCK_02;
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		m_team = BLOCK_TYPE::BLOCK_01;
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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
			break;
		}
		case CObject::TYPE::BULLET:
			break;
		default:
			break;
		}

		// ��x���폜����Ȃ���΁A
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
