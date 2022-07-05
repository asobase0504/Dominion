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
class COperate;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CCharacter : public CObject2D
{
public:
	CCharacter();
	~CCharacter();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// ���ߎ҂̐ݒ�
	void SetOperate(COperate* inOperate) { m_operate = inOperate; }

	static CCharacter* Create();
private:
	COperate* m_operate;	// ���߂��o���l
};
#endif // !_CHARACTER_H_
