//=========================================
// 
// モードクラス
// Author YudaKaito
// 
//=========================================
#ifndef _MODE_H_
#define _MODE_H_

#include <d3dx9.h>

//-----------------------------------------
// モードクラス
//-----------------------------------------
class CMode
{
public:	// メンバー関数
	CMode();
	virtual ~CMode();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private: // 動的メンバー変数
};

#endif // !_GAME_H_
