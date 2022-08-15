//=============================================================================
// 
// 色管理クラス
// Author YudaKaito
// 
//=============================================================================
#ifndef _COLOR_H_
#define _COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// 色管理クラス
//-----------------------------------------------------------------------------
class CColor
{
public:	// メンバー関数
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(int inConfig);

	// Setter

private: // 動的メンバー変数
	std::map<int, D3DXCOLOR> m_color;
};

#endif // !_COLOR_H_
