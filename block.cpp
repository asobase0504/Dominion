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
		block->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));
		break;
	case BLOCK_TYPE::BLOCK_01:
		block->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		block->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
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
		SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		m_team = BLOCK_TYPE::BLOCK_01;
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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
