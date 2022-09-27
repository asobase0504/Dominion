//=========================================
// 
// �X�e�[�W�I���̃��j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _NUMBER_SELECT_MENU_H_
#define _NUMBER_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CNumberSelectMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CNumberSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CNumberSelectMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

	void SetAbovePasteTexture(std::string inTag);
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	CObject2D* m_EmphasisSelect;
	CObject2D* m_abovePaste;
	int m_SinTime;
};

#endif // !_NUMBER_SELECT_MENU_H_