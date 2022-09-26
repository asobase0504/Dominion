//=========================================
// 
// ステージ選択のメニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _COLOR_SELECT_MENU_H_
#define _COLOR_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// メニュークラス
//-----------------------------------------
class CColorSelectMenu :public CMenu
{
public:	// パブリック関数
	CColorSelectMenu();
	~CColorSelectMenu();

	void UpdateBeforeDecision() override;
	void UpdateAfterDecision() override;

	static CColorSelectMenu* Create(int inColorCount);
};

//-----------------------------------------
// フレームクラス
//-----------------------------------------
class CColorSelectMenuFream : public CMenuFream
{
public:	// パブリック関数
	CColorSelectMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CColorSelectMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // プライベート関数

private: // メンバー変数

};

//-----------------------------------------
// 項目クラス
//-----------------------------------------
class CColorSelectMenuItem : public CMenuItem
{
public:	// パブリック関数
	CColorSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CColorSelectMenuItem() override;

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
