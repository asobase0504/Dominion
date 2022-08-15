//=============================================================================
// 
// �F�Ǘ��N���X
// Author YudaKaito
// 
//=============================================================================
#ifndef _COLOR_H_
#define _COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// �F�Ǘ��N���X
//-----------------------------------------------------------------------------
class CColor
{
public:	// �����o�[�֐�
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(int inConfig);

	// Setter

private: // ���I�����o�[�ϐ�
	std::map<int, D3DXCOLOR> m_color;
};

#endif // !_COLOR_H_
