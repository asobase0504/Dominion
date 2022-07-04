//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _OPERATE_H_
#define _OPERATE_H_

//-----------------------------------------
// include
//-----------------------------------------
#include <d3dx9.h>

//-----------------------------------------
// �I�y���[�^�[�N���X
//-----------------------------------------
class COperate
{
public:
	enum TYPE
	{
		MOVE = 0,
		BULLET_SHOT
	};
public:
	COperate();
	~COperate();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	virtual D3DXVECTOR3 Move() = 0;
	virtual bool BulletShot() = 0;
private:

};
#endif // !_OPERATE_H_
