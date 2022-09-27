//=============================================================================
// 
// �l���I�����j���[�N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "menu_number_select.h"
#include "application.h"

//=============================================================================
// 
// �l���I�����j���[�A�C�e���N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CNumberSelectMenuItem::CNumberSelectMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_SinTime(0),
	m_abovePaste(nullptr)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CNumberSelectMenuItem::~CNumberSelectMenuItem()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CNumberSelectMenuItem::Init()
{
	{
		m_EmphasisSelect = new CObject2D;
		m_EmphasisSelect->Init();
		m_EmphasisSelect->SetSize(D3DXVECTOR2(0.0f, 0.0f));
		m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	}
	CMenuItem::Init();
	{
		m_abovePaste = new CObject2D(CObject::TYPE::NONE,5);
		m_abovePaste->Init();
		m_abovePaste->SetSize(D3DXVECTOR2(0.0f, 0.0f));
		m_abovePaste->SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::Uninit()
{
	if (m_EmphasisSelect != nullptr)
	{
		m_EmphasisSelect->SetIsDeleted(true);
		m_EmphasisSelect = nullptr;
	}
	if (m_abovePaste != nullptr)
	{
		m_abovePaste->SetIsDeleted(true);
		m_abovePaste = nullptr;
	}
	CMenuItem::Uninit();
}

//-----------------------------------------------------------------------------
// �o����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::PopUpdate()
{
	m_EmphasisSelect->SetPos(m_pos);
	m_abovePaste->SetSize(D3DXVECTOR2(80.0f, 80.0f));
	m_abovePaste->SetPos(m_pos);
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::SelectUpdate()
{
	m_SinTime++;
	SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	D3DXVECTOR2 size(85.0f, 85.0f);
	size.x += 10.0f * sinf(0.095f * m_SinTime);
	size.y += 10.0f * sinf(0.095f * m_SinTime);
	m_EmphasisSelect->SetSize(size);
	m_EmphasisSelect->SetColorAlpha(1.0f);
	m_abovePaste->SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------------------------------------------
// �ʏ��Ԃ̍X�V
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::NomalUpdate()
{
	m_SinTime = 0;
	SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
	m_EmphasisSelect->SetColorAlpha(0.0f);
	m_abovePaste->SetColor(CApplication::GetInstance()->GetColor(1));
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::EndUpdate()
{
}

//-----------------------------------------------------------------------------
// ��ɓ\��I�u�W�F�N�g�̃e�N�X�`���ݒ�
//-----------------------------------------------------------------------------
void CNumberSelectMenuItem::SetAbovePasteTexture(std::string inTag)
{
	m_abovePaste->SetTexture(inTag);
}
