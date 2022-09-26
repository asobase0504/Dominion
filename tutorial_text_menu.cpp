//=============================================================================
// 
// �e�N�X�`���I�����j���[�N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "tutorial_text_menu.h"
#include "application.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenu::CTutorialTextMenu()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenu::~CTutorialTextMenu()
{
}

//-----------------------------------------------------------------------------
// ����O�̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenu::UpdateBeforeDecision()
{
	CMenu::UpdateBeforeDecision();
	// �t���[���F�̐ݒ�
	m_fream->SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------------------------------------------
// �����̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenu::UpdateAfterDecision()
{
}

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CTutorialTextMenu* CTutorialTextMenu::Create()
{
	// �t���[���̐ݒ�
	CMenuFream* fream = new CTutorialTextMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
//	for (int i = 0; i < inColorCount; i++)
	{

		CMenuItem* item = new CTutorialTextMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(60.0f, 60.0f));			// �傫���̐ݒ�
		item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;

	D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
	CTutorialTextMenu* menu = (CTutorialTextMenu*)CMenu::Create(pos, area, fream);
	menu->SetInterval({ 25.0f,25.0f });
	menu->SetItems(items);

	return menu;
}

//=============================================================================
// 
// �`�[���I�����j���[�t���[���N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenuFream::CTutorialTextMenuFream(CObject::TYPE type) :
	CMenuFream(type)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenuFream::~CTutorialTextMenuFream()
{
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTutorialTextMenuFream::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenuFream::Update()
{
}

//=============================================================================
// 
// �`�[���I�����j���[�A�C�e���N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenuItem::CTutorialTextMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_SinTime(0)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTutorialTextMenuItem::~CTutorialTextMenuItem()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CTutorialTextMenuItem::Init()
{
	{
		m_EmphasisSelect = new CObject2D;
		m_EmphasisSelect->Init();
		m_EmphasisSelect->SetSize(D3DXVECTOR2(80.0f, 80.0f));
		m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	}
	CMenuItem::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTutorialTextMenuItem::Uninit()
{
	if (m_EmphasisSelect != nullptr)
	{
		m_EmphasisSelect->SetIsDeleted(true);
		m_EmphasisSelect->Uninit();
		m_EmphasisSelect = nullptr;
	}
	CMenuItem::Uninit();
}

//-----------------------------------------------------------------------------
// �o����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenuItem::PopUpdate()
{
	m_EmphasisSelect->SetPos(m_pos);
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenuItem::SelectUpdate()
{
	m_SinTime++;
	SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
	D3DXVECTOR2 size(80.0f, 80.0f);
	size.x += 5.0f * sinf(0.095f * m_SinTime);
	size.y += 5.0f * sinf(0.095f * m_SinTime);
	m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));
	m_EmphasisSelect->SetSize(size);
	m_EmphasisSelect->SetColorAlpha(1.0f);
}

//-----------------------------------------------------------------------------
// �ʏ��Ԃ̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenuItem::NomalUpdate()
{
	m_SinTime = 0;
	SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	m_EmphasisSelect->SetColorAlpha(0.0f);
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CTutorialTextMenuItem::EndUpdate()
{
}
