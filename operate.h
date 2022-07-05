//=========================================
// 
// プレイヤークラス
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
// オペレータークラス
//-----------------------------------------
class COperate
{
public:
	enum TYPE
	{
		MOVE = 0,
		BULLET_SHOT
	};

	enum SHOT_TYPE
	{
		UP_SHOT = 0,
		DOWN_SHOT,
		LEFT_SHOT,
		RIGHT_SHOT,
		NONE_SHOT
	};
public:
	COperate();
	~COperate();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	virtual D3DXVECTOR3 Move() = 0;
	virtual SHOT_TYPE BulletShot() = 0;
private:

};
#endif // !_OPERATE_H_
