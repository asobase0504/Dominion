//=========================================
// Controller
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_OPERATE_H_
#define _PLAYER_OPERATE_H_

#include "controller.h"

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CPlayerController : public CController
{
public:
	CPlayerController(int inInputIndex);
	~CPlayerController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;

private:
	int m_nInputIdx;
};

#endif // !_PLAYER_OPERATE_H_
