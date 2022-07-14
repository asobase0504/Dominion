//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CController;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CCharacter : public CObject2D
{
public:
	CCharacter();
	~CCharacter() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// ���ߎ҂̐ݒ�
	void SetController(CController* inOperate) { m_controller = inOperate; }

	static CCharacter* Create();

private:
	CController* m_controller;	// ���߂��o���l
};

#endif // !_CHARACTER_H_
