//=========================================
// 
// �^�C�g�����X�e�[�^�X�̊�{�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_STATUS_H_
#define _TITLE_STATUS_H_

#include <d3dx9.h>

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CTitleStatus
{
public:	// �����o�[�֐�
	CTitleStatus();
	virtual ~CTitleStatus() = 0;

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private: // ���I�����o�[�ϐ�
};

#endif // !_TITLE_H_
