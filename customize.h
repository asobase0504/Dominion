//=========================================
// 
// カスタマイズ内クラス
// Author YudaKaito
// 
//=========================================
#ifndef _CUSTOMIZE_H_
#define _CUSTOMIZE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CObject2D;
class CMenu;
class CMap;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CCustomize : public CMode
{
public:	// メンバー関数
	CCustomize();
	~CCustomize() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // 動的メンバー変数
	CObject2D* m_bg;	// 背景
	CMenu* m_manu;		// メニュー
	CMap* m_stage;		// ステージ
};

#endif // !_TITLE_H_
