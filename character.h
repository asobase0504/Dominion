//=========================================
// 
// �v���C���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CController;

//-----------------------------------------
// �v���C���[�N���X
//-----------------------------------------
class CCharacter : public CObject2D
{
public: // �񋓌^
	enum TEAM
	{
		TEAM_00,
		TEAM_01
	};

public:
	CCharacter();
	~CCharacter() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// ���ߎ҂̐ݒ�
	void SetController(CController* inOperate) { m_controller = inOperate; }
	void SetTeam(const TEAM inTeam);

	static CCharacter* Create(TEAM inTeam);

private: // �v���C�x�[�g�֐�
	void ScreenFromOutTime();

private:
	CController* m_controller;	// ���߂��o���l
	TEAM m_team;				// �������Ă�`�[��
};

#endif // !_CHARACTER_H_
