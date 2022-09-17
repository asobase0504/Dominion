//=========================================
// 
// ���j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// �|�[�Y���̍��ڃN���X
//-----------------------------------------
class CPauseMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CPauseMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CPauseMenuItem() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

	void SetText();
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	CObject2D* m_text;
};

#endif // !_CHARACTER_H_
