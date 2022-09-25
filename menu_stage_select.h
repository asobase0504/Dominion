//=========================================
// 
// ステージ選択のメニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _STAGE_SELECT_MENU_H_
#define _STAGE_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// メニュークラス
//-----------------------------------------
class CStageSelectMenu :public CMenu
{
public:	// パブリック関数
	CStageSelectMenu();
	~CStageSelectMenu();

	void UpdateBeforeDecision();
	void UpdateAfterDecision();

	static CStageSelectMenu* Create(int inStageCount);
};

//-----------------------------------------
// フレームクラス
//-----------------------------------------
class CStageSelectMenuFream : public CMenuFream
{
public:	// パブリック関数
	CStageSelectMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CStageSelectMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // プライベート関数

private: // メンバー変数

};

//-----------------------------------------
// 項目クラス
//-----------------------------------------
class CStageSelectMenuItem : public CMenuItem
{
public:	// パブリック関数
	CStageSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CStageSelectMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

private: // プライベート関数

private: // メンバー変数
	CObject2D* m_EmphasisSelect;
	int m_SinTime;
};

#endif // !_CHARACTER_H_
