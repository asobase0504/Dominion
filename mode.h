//=========================================
// 
// ���[�h�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _MODE_H_
#define _MODE_H_

#include <d3dx9.h>

//-----------------------------------------
// ���[�h�N���X
//-----------------------------------------
class CMode
{
public:	// �����o�[�֐�
	CMode();
	virtual ~CMode();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private: // ���I�����o�[�ϐ�
};

#endif // !_GAME_H_
