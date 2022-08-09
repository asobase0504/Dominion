//=========================================
// 
// �c�e�̕\���N���X
// Author YudaKaito
// 
//=========================================
#ifndef _RENAUBS_BULLET_H_
#define _RENAUBS_BULLET_H_

#include "object2d.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CCharacter;

//-----------------------------------------
// �c�e�\���N���X
//-----------------------------------------
class CRemaubsBullet : public CObject2D
{
public: // �񋓌^
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
