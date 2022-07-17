//=========================================
// Controller
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_OPERATE_H_
#define _PLAYER_OPERATE_H_

#include "controller.h"

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CPlayerController : public CController
{
public:
	CPlayerController();
	~CPlayerController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;

private:

};

#endif // !_PLAYER_OPERATE_H_