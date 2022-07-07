//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _AI_OPERATE_H_
#define _AI_OPERATE_H_

#include "operate.h"

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CAIOperate : public COperate
{
public:
	CAIOperate();
	~CAIOperate() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;
private:

};

#endif // !_AI_OPERATE_H_
