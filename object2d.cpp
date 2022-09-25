//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#include "object2d.h"
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject2D::CObject2D(TYPE type,int inPriority) :
	CObject(type, inPriority),
	m_pVtxBuff(nullptr),
	m_texture("\0"),
	m_rotY(0.0f),
	m_fLength(0.0f),
	m_fAngle(0.0f),
	m_anchor(CENTER)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CObject2D::Init()
{
	// ���_�o�b�t�@�̐���
	CApplication::GetInstance()->GetRenderer()->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;
	m_fAngle = atan2f(m_size.x, m_size.y);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = CApplication::GetInstance()->CENTER_X + sinf(m_rotY + m_fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.y = CApplication::GetInstance()->CENTER_Y + cosf(m_rotY + m_fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = CApplication::GetInstance()->CENTER_X + sinf(m_rotY + -m_fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = CApplication::GetInstance()->CENTER_Y + cosf(m_rotY + -m_fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = CApplication::GetInstance()->CENTER_X + sinf(m_rotY + m_fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.y = CApplication::GetInstance()->CENTER_Y + cosf(m_rotY + m_fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = CApplication::GetInstance()->CENTER_X + sinf(m_rotY + m_fAngle) * m_fLength;
	pVtx[3].pos.y = CApplication::GetInstance()->CENTER_Y + cosf(m_rotY + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// tex�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CObject2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	m_isDeleted = true;
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CObject2D::Update()
{
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 texture = CApplication::GetInstance()->GetTextureClass()->GetTexture(m_texture);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �e�N�X�`���̔j��
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CObject2D::SetPos(const D3DXVECTOR3 & inPos)
{
	CObject::SetPos(inPos);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_anchor)
	{
	case CObject2D::CENTER:
		pVtx[0].pos.x = m_pos.x - m_size.x * 0.5f;
		pVtx[0].pos.y = m_pos.y - m_size.y * 0.5f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + m_size.x * 0.5f;
		pVtx[1].pos.y = m_pos.y - m_size.y * 0.5f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x - m_size.x * 0.5f;
		pVtx[2].pos.y = m_pos.y + m_size.y * 0.5f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_size.x * 0.5f;
		pVtx[3].pos.y = m_pos.y + m_size.y * 0.5f;
		pVtx[3].pos.z = 0.0f;
		break;
	case CObject2D::TOP:
		break;
	case CObject2D::DOWN:
		break;
	case CObject2D::LEFT:
		break;
	case CObject2D::RIGHT:
		break;
	case CObject2D::TOPLEFT:
		pVtx[0].pos.x = m_pos.x - m_size.x;
		pVtx[0].pos.y = m_pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x;
		pVtx[1].pos.y = m_pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x - m_size.x;
		pVtx[2].pos.y = m_pos.y + m_size.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x;
		pVtx[3].pos.y = m_pos.y + m_size.y;
		pVtx[3].pos.z = 0.0f;
		break;
	case CObject2D::TOPRIGHT:
		pVtx[0].pos.x = m_pos.x;
		pVtx[0].pos.y = m_pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + m_size.x;
		pVtx[1].pos.y = m_pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x;
		pVtx[2].pos.y = m_pos.y + m_size.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_size.x;
		pVtx[3].pos.y = m_pos.y + m_size.y;
		pVtx[3].pos.z = 0.0f;
		break;
	case CObject2D::DOWNLEFT:
		pVtx[0].pos.x = m_pos.x - m_size.x;
		pVtx[0].pos.y = m_pos.y - m_size.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x;
		pVtx[1].pos.y = m_pos.y - m_size.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x - m_size.x;
		pVtx[2].pos.y = m_pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x;
		pVtx[3].pos.y = m_pos.y;
		pVtx[3].pos.z = 0.0f;
		break;
	case CObject2D::DOWNRIGHT:
		pVtx[0].pos.x = m_pos.x;
		pVtx[0].pos.y = m_pos.y - m_size.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + m_size.x;
		pVtx[1].pos.y = m_pos.y - m_size.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x;
		pVtx[2].pos.y = m_pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_size.x;
		pVtx[3].pos.y = m_pos.y;
		pVtx[3].pos.z = 0.0f;
		break;
	default:
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �傫���̐ݒ�
//=============================================================================
void CObject2D::SetSize(const D3DXVECTOR2 & inSize)
{
	m_size = inSize;

	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;
	m_fAngle = atan2f(m_size.x, m_size.y);

	SetPos(m_pos);
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CObject2D::SetColor(const D3DXCOLOR & inColor)
{
	m_col = inColor;

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = inColor;
	pVtx[1].col = inColor;
	pVtx[2].col = inColor;
	pVtx[3].col = inColor;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �����F�݂̂̐ݒ�
//=============================================================================
void CObject2D::SetColorAlpha(const float inAlpha)
{
	m_col.a = inAlpha;

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S���N���X�̍쐬
//=============================================================================
CObject2D* CObject2D::Create(TYPE type, int inPriority)
{
	CObject2D* objectCreate = new CObject2D(type, inPriority);

	if ((objectCreate == nullptr) || FAILED(objectCreate->Init()))
	{
		assert(false);
	}

	return objectCreate;
}
