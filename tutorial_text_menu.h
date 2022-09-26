//=========================================
// 
// �`���[�g���A���e�L�X�g�̃��j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _TUTORIAL_TEXT_MENU_H_
#define _TUTORIAL_TEXT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CTutorialTextMenu :public CMenu
{
public:
	enum MODE
	{
		MOVE = 0,
		SHOT,
		CHARGE_SHOT
	};
public:	// �p�u���b�N�֐�
	CTutorialTextMenu();
	~CTutorialTextMenu();

	void UpdateBeforeDecision() override;
	void UpdateAfterDecision() override;

	static CTutorialTextMenu* Create();
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CTutorialTextMenuFream : public CMenuFream
{
public:	// �p�u���b�N�֐�
	CTutorialTextMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CTutorialTextMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CTutorialTextMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CTutorialTextMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CTutorialTextMenuItem() override;

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
