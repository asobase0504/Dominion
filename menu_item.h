//=========================================
// 
// メニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// ポーズ中の項目クラス
//-----------------------------------------
class CPauseMenuItem : public CMenuItem
{
public:	// パブリック関数
	CPauseMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CPauseMenuItem() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

	void SetText();
private: // プライベート関数

private: // メンバー変数
	CObject2D* m_text;
};

#endif // !_CHARACTER_H_
