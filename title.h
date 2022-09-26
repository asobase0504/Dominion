//=========================================
// 
// �^�C�g�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CMenu;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CTitle : public CMode
{
public: // �񋓌^
	enum class Status
	{
		GAME_STAET,
		TUTORIAL,
		CUSTOMIZE,
		OPSITON,
		MAX,
		NONE
	};
public:	// �����o�[�֐�
	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // ���I�����o�[�ϐ�
	CMenu* m_manu;
	Status m_status;
	int m_decisionDvice;
};

#endif // !_TITLE_H_
