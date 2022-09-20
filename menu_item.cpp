//=========================================
// 
// ���ڃN���X
// Author YudaKaito
// 
//=========================================
#include "menu_item.h"
#include "application.h"

//=========================================
// 
// �|�[�Y�����ڃN���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CPauseMenuItem::CPauseMenuItem(CObject::TYPE type) :
	CMenuItem(type),
	m_text(nullptr)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CPauseMenuItem::~CPauseMenuItem()
{
}

//-----------------------------------------
// �o����Ԃ̍X�V
//-----------------------------------------
void CPauseMenuItem::PopUpdate()
{
}

//-----------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------
void CPauseMenuItem::SelectUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(0));
}

//-----------------------------------------
// �ʏ��Ԃ̍X�V
//-----------------------------------------
void CPauseMenuItem::NomalUpdate()
{
	SetColor(CApplication::GetInstance()->GetColor(2));
}

//-----------------------------------------
// �I����Ԃ̍X�V
//-----------------------------------------
void CPauseMenuItem::EndUpdate()
{
}
