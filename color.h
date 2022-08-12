//=========================================
// 
// 色管理クラス
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>

//-----------------------------------------
// 色管理クラス
//-----------------------------------------
class CColor
{
public:
	enum class COLOR_CONFIG
	{
		COLOR_0 = 0,
		COLOR_1,
		COLOR_2,
		MAX
	};

public:	// メンバー関数
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(COLOR_CONFIG inConfig);

	// Setter

private: // 動的メンバー変数
	D3DXCOLOR color;
};

#endif // !_APPLICATION_H_
