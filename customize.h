//=========================================
// 
// �J�X�^�}�C�Y���N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CUSTOMIZE_H_
#define _CUSTOMIZE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CObject2D;
class CMenu;
class CMap;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CCustomize : public CMode
{
public:	// �����o�[�֐�
	CCustomize();
	~CCustomize() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // ���I�����o�[�ϐ�
	CObject2D* m_bg;	// �w�i
	CMenu* m_manu;		// ���j���[
	CMap* m_stage;		// �X�e�[�W
};

#endif // !_TITLE_H_
