//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _AI_OPERATE_H_
#define _AI_OPERATE_H_

#include "controller.h"

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CAIController : public CController
{
public:
	CAIController();
	~CAIController() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	SHOT_TYPE BulletShot() override;
private:
	bool isBulletShot;
};

#endif // !_AI_OPERATE_H_
