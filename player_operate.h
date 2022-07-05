//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "operate.h"

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CPlayerOperate : public COperate
{
public:
	CPlayerOperate();
	~CPlayerOperate();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	bool BulletShot() override;

private:

};
#endif // !_PLAYER_H_
