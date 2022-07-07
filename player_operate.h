//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_OPERATE_H_
#define _PLAYER_OPERATE_H_

#include "operate.h"

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CPlayerOperate : public COperate
{
public:
	CPlayerOperate();
	~CPlayerOperate() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;

private:

};

#endif // !_PLAYER_OPERATE_H_
