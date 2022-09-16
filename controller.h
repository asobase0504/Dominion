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
#include "character.h"

//-----------------------------------------
// オペレータークラス
//-----------------------------------------
class CController
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
		UP_CHARGE_SHOT,
		DOWN_CHARGE_SHOT,
		LEFT_CHARGE_SHOT,
		RIGHT_CHARGE_SHOT,
		CHARGE_NOW,
		NONE_SHOT
	};
public:
	CController();
	virtual ~CController();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	virtual D3DXVECTOR3 Move() = 0;
	virtual SHOT_TYPE BulletShot() = 0;

	// Setter
	void SetToOrder(CCharacter* inOrder) { m_toOrder = inOrder; }
protected:
	CCharacter* m_toOrder;	// 命令先
	int chargeCount;
};

#endif // !_OPERATE_H_
