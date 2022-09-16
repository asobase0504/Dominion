//=============================================================================
// 
// 色管理クラス
// Author YudaKaito
// 
//=============================================================================
#ifndef _THEME_COLOR_H_
#define _THEME_COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// 色管理クラス
//-----------------------------------------------------------------------------
class CThemeColor
{
public:	// メンバー関数
	CThemeColor();
	~CThemeColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(int inConfig);
	int GetSize() { return typeNumber; }

	// Setter
	void SetTheme(int idx);

private: // 動的メンバー変数
	std::map<int, D3DXCOLOR> m_color;
	int typeNumber;
};

#endif // !_COLOR_H_
