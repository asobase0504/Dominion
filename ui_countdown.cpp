//=============================================================================
//
// �Q�[���J�n���̃J�E���g�_�E���N���X
// Author YudaKaito
//
//=============================================================================
#include "application.h"
#include "ui_countdown.h"

const int CCountDownUI::READY_TIME = 60;		// REDEY�I���܂ł̎���
const int CCountDownUI::GO_TIME = 30;			// GO�I���܂ł̎���
const int CCountDownUI::PRIORITY_BELONG = 5;	// �v���C�I���e�B

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CCountDownUI * CCountDownUI::Create(const D3DXVECTOR2 & inPos)
{
	CCountDownUI* countDown = new CCountDownUI;

	if (FAILED(countDown->Init()))
	{
		return nullptr;
	}

	// �t���[���̏�����
	{
		countDown->m_fream = new CObject2D(CObject::TYPE::NONE, PRIORITY_BELONG);
		if (FAILED(countDown->m_fream->Init()))
		{
			return nullptr;
		}
		countDown->m_fream->SetPos((D3DXVECTOR3)inPos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(0);
		color.a = 0.95f;
		countDown->m_fream->SetColor(color);
		countDown->m_fream->SetSize({ (float)CApplication::GetInstance()->SCREEN_WIDTH,100.0f });
		countDown->m_fream->SetShouldStopAlsoUpdate();
	}

	// �e�L�X�g�̏�����
	{
		countDown->m_text = new CObject2D(CObject::TYPE::NONE, 5);
		if (FAILED(countDown->m_text->Init()))
		{
			return nullptr;
		}
		countDown->m_text->SetPos((D3DXVECTOR3)inPos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(1);
		color.a = 0.95f;
		countDown->m_text->SetColor(color);
		countDown->m_text->SetSize({ 300.0f,80.0f });
		countDown->m_text->SetTexture("TEXT_READY");
		countDown->m_text->SetShouldStopAlsoUpdate();
	}

	return countDown;
}

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCountDownUI::CCountDownUI() :
	m_pos({0.0f,0.0f}),
	m_fream(nullptr),
	m_text(nullptr),
	m_count(0),
	m_isDeleted(false)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCountDownUI::~CCountDownUI()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CCountDownUI::Init()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CCountDownUI::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CCountDownUI::Update()
{
	//CObject::SetStopUpdate(true);
	m_count++;

	if (m_count >= READY_TIME)
	{
		m_text->SetTexture("TEXT_GO");
	}
	if (m_count >= READY_TIME + GO_TIME)
	{
		//CObject::SetStopUpdate(false);
		SetIsDeleted();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CCountDownUI::Draw()
{
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�����S������
//-----------------------------------------------------------------------------
void CCountDownUI::SetIsDeleted()
{
	m_isDeleted = true;
	m_text->SetIsDeleted(true);
	m_fream->SetIsDeleted(true);
}
