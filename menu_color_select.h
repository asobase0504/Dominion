//=========================================
// 
// �X�e�[�W�I���̃��j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _COLOR_SELECT_MENU_H_
#define _COLOR_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CColorSelectMenu :public CMenu
{
public:	// �p�u���b�N�֐�
	CColorSelectMenu();
	~CColorSelectMenu();

	void UpdateBeforeDecision() override;
	void UpdateAfterDecision() override;

	static CColorSelectMenu* Create(int inColorCount);
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CColorSelectMenuFream : public CMenuFream
{
public:	// �p�u���b�N�֐�
	CColorSelectMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CColorSelectMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CColorSelectMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CColorSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CColorSelectMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	CObject2D* m_EmphasisSelect;
	int m_SinTime;
};

#endif // !_CHARACTER_H_
