//=========================================
// 
// タイトル内ステータスの基本クラス
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_STATUS_H_
#define _TITLE_STATUS_H_

#include <d3dx9.h>

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CTitleStatus
{
public:	// メンバー関数
	CTitleStatus();
	virtual ~CTitleStatus() = 0;

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private: // 動的メンバー変数
};

#endif // !_TITLE_H_
