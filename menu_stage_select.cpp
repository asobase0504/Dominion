//=============================================================================
// 
// �X�e�[�W�I�����j���[�N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "menu_stage_select.h"
#include "application.h"
#include "sound.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CStageSelectMenu::CStageSelectMenu()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CStageSelectMenu::~CStageSelectMenu()
{
}

//-----------------------------------------------------------------------------
// ����O�̍X�V
//-----------------------------------------------------------------------------
void CStageSelectMenu::UpdateBeforeDecision()
{
	CMenu::UpdateBeforeDecision();
}

//-----------------------------------------------------------------------------
// �����̍X�V
//-----------------------------------------------------------------------------
void CStageSelectMenu::UpdateAfterDecision()
{
}

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CStageSelectMenu* CStageSelectMenu::Create(int inStageCount)
{
	// �t���[���̐ݒ�
	CMenuFream* fream = new CMenuFream;
	{
		fream->Init();
		fream->SetColor(CApplication::GetInstance()->GetColor(0));
		fream->SetColorAlpha(0.55f);
	}

	std::vector<std::vector<CMenuItem*>> items;
	std::vector<CMenuItem*> X;
	for (int i = 0; i < inStageCount; i++)
	{

		CMenuItem* item = new CStageSelectMenuItem;
		item->Init();
		item->SetSize(D3DXVECTOR2(60.0f, 60.0f));			// �傫���̐ݒ�
		item->SetColor(CApplication::GetInstance()->GetColor(0));			// �F�̐ݒ�

		X.push_back(item);
	}
	items.push_back(X);

	D3DXVECTOR2 area = CApplication::GetInstance()->GetScreenSize();
	area.y *= 0.25f;

	D3DXVECTOR2 pos(CApplication::GetInstance()->GetScreenCenter());
	CStageSelectMenu* menu = (CStageSelectMenu*)CMenu::Create(pos, area, fream);
	menu->SetInterval({25.0f,25.0f});
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
CStageSelectMenuFream::CStageSelectMenuFream(CObject::TYPE type) :
	CMenuFream(type)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CStageSelectMenuFream::~CStageSelectMenuFream()
{
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CStageSelectMenuFream::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CStageSelectMenuFream::Update()
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
CStageSelectMenuItem::CStageSelectMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_SinTime(0)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CStageSelectMenuItem::~CStageSelectMenuItem()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CStageSelectMenuItem::Init()
{
	{
		m_EmphasisSelect = new CObject2D;
		m_EmphasisSelect->Init();
		m_EmphasisSelect->SetSize(D3DXVECTOR2(0.0f, 0.0f));
		m_EmphasisSelect->SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	}
	CMenuItem::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::Uninit()
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
void CStageSelectMenuItem::PopUpdate()
{
	m_EmphasisSelect->SetPos(m_pos);
	m_isPopNow = false;
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::SelectUpdate()
{
	m_SinTime++;
	SetColor(CApplication::GetInstance()->GetColor(0));	// �F�̐ݒ�
	D3DXVECTOR2 size(80.0f, 80.0f);
	size.x += 5.0f * sinf(0.095f * m_SinTime);
	size.y += 5.0f * sinf(0.095f * m_SinTime);
	m_EmphasisSelect->SetSize(size);
	m_EmphasisSelect->SetColorAlpha(1.0f);
}

//-----------------------------------------------------------------------------
// �ʏ��Ԃ̍X�V
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::NomalUpdate()
{
	m_SinTime = 0;
	SetColor(CApplication::GetInstance()->GetColor(1));	// �F�̐ݒ�
	m_EmphasisSelect->SetColorAlpha(0.0f);
}

//-----------------------------------------------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------------------------------------------
void CStageSelectMenuItem::EndUpdate()
{
}
