//=========================================
// 
// チュートリアルテキストのメニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _TUTORIAL_TEXT_MENU_H_
#define _TUTORIAL_TEXT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// メニュークラス
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
public:	// パブリック関数
	CTutorialTextMenu();
	~CTutorialTextMenu();

	void UpdateBeforeDecision() override;
	void UpdateAfterDecision() override;

	static CTutorialTextMenu* Create();
};

//-----------------------------------------
// フレームクラス
//-----------------------------------------
class CTutorialTextMenuFream : public CMenuFream
{
public:	// パブリック関数
	CTutorialTextMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CTutorialTextMenuFream() override;

	virtual void Uninit() override;
	virtual void Update() override;

private: // プライベート関数

private: // メンバー変数

};

//-----------------------------------------
// 項目クラス
//-----------------------------------------
class CTutorialTextMenuItem : public CMenuItem
{
public:	// パブリック関数
	CTutorialTextMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CTutorialTextMenuItem() override;

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
