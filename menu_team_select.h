//=========================================
// 
// �`�[���I���̃��j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CTeamSelectMenu :public CMenu
{
public:	// �p�u���b�N�֐�
	CTeamSelectMenu();
	~CTeamSelectMenu();

	void UpdateBeforeDecision();
	void UpdateAfterDecision();
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CTeamSelectMenuFream : public CMenuFream
{
public:	// �p�u���b�N�֐�
	CTeamSelectMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CTeamSelectMenuFream() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CTeamSelectMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CTeamSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CTeamSelectMenuItem() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	CObject2D* m_text;
};

#endif // !_CHARACTER_H_
