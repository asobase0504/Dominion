//=========================================
// 
// タイトル内クラス
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CTitle : public CMode
{
public:	// メンバー関数
	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // 動的メンバー変数
};

#endif // !_TITLE_H_
