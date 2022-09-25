//=========================================
// 
// �X�e�[�W�I���̃��j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _STAGE_SELECT_MENU_H_
#define _STAGE_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CStageSelectMenu :public CMenu
{
public:	// �p�u���b�N�֐�
	CStageSelectMenu();
	~CStageSelectMenu();

	void UpdateBeforeDecision();
	void UpdateAfterDecision();

	static CStageSelectMenu* Create(int inStageCount);
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CStageSelectMenuFream : public CMenuFream
{
public:	// �p�u���b�N�֐�
	CStageSelectMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CStageSelectMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CStageSelectMenuItem : public CMenuItem
{
public:	// �p�u���b�N�֐�
	CStageSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CStageSelectMenuItem() override;

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
