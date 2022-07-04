//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "operate.h"

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CPlayer : public COperate
{
public:
	CPlayer();
	~CPlayer();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	D3DXVECTOR3 Move() override;
	bool BulletShot() override;

private:

};
#endif // !_PLAYER_H_
