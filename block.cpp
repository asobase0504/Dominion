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
	m_type(TYPE::NONE)
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
	//CObject2D::Update();
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
CBlock* CBlock::Create(TYPE type)
{
	CBlock* block = new CBlock;

	if (block == nullptr)
	{
		return nullptr;
	}

	block->Init();

	switch (type)
	{
	case TYPE::NONE:
		block->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));
		break;
	case TYPE::BLOCK_01:
		block->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case TYPE::BLOCK_02:
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
void CBlock::ChangeType(TYPE type)
{
	switch (m_type)
	{
	case TYPE::NONE:
		break;
	case TYPE::BLOCK_01:
		m_type = TYPE::BLOCK_02;
		break;
	case TYPE::BLOCK_02:
		m_type = TYPE::BLOCK_01;
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
CBlock::TYPE CBlock::GetType()
{
	return m_type;
}
