//=============================================================================
// 
// �F�Ǘ��N���X
// Author YudaKaito
// 
//=============================================================================
#ifndef _THEME_COLOR_H_
#define _THEME_COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// �F�Ǘ��N���X
//-----------------------------------------------------------------------------
class CThemeColor
{
public:	// �����o�[�֐�
	CThemeColor();
	~CThemeColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(int inConfig);

	// Setter
	void SetTheme(int idx);

private: // ���I�����o�[�ϐ�
	std::map<int, D3DXCOLOR> m_color;
};

#endif // !_COLOR_H_
