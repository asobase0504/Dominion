//=========================================
// 
// �^�C�g�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CTitle : public CMode
{
public:	// �����o�[�֐�
	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // ���I�����o�[�ϐ�
};

#endif // !_TITLE_H_
