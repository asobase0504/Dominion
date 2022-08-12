//=========================================
// 
// �F�Ǘ��N���X
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>

//-----------------------------------------
// �F�Ǘ��N���X
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

public:	// �����o�[�֐�
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter
	D3DXCOLOR GetColor(COLOR_CONFIG inConfig);

	// Setter

private: // ���I�����o�[�ϐ�
	D3DXCOLOR color;
};

#endif // !_APPLICATION_H_
