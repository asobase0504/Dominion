//=========================================
// 
// ステージ選択のメニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _NUMBER_SELECT_MENU_H_
#define _NUMBER_SELECT_MENU_H_

#include "object2d.h"
#include "menu.h"
#include <vector>

//-----------------------------------------
// 項目クラス
//-----------------------------------------
class CNumberSelectMenuItem : public CMenuItem
{
public:	// パブリック関数
	CNumberSelectMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CNumberSelectMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;

	void PopUpdate() override;
	void SelectUpdate() override;
	void NomalUpdate() override;
	void EndUpdate() override;

	void SetAbovePasteTexture(std::string inTag);
private: // プライベート関数

private: // メンバー変数
	CObject2D* m_EmphasisSelect;
	CObject2D* m_abovePaste;
	int m_SinTime;
};

#endif // !_NUMBER_SELECT_MENU_H_