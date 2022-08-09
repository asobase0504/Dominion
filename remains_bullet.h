//=========================================
// 
// 残弾の表示クラス
// Author YudaKaito
// 
//=========================================
#ifndef _RENAUBS_BULLET_H_
#define _RENAUBS_BULLET_H_

#include "object2d.h"

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CCharacter;

//-----------------------------------------
// 残弾表示クラス
//-----------------------------------------
class CRemaubsBullet : public CObject2D
{
public: // 列挙型
	enum TEAM
	{
		TEAM_00,
		TEAM_01
	};

public:
	CRemaubsBullet();
	~CRemaubsBullet() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CRemaubsBullet* Create(CCharacter* inPerent, float inRotY);

private:
	CCharacter* Parent;
	float rotY;
};

#endif // !_RENAUBS_BULLET_H_
