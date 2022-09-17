//=========================================
// 
// �|�[�Y���N���X
// Author YudaKaito
// 
//=========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include <Windows.h>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CObject2D;
class CMenu;

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CPause
{
public: // �񋓌^
	enum Status
	{
		BACK = 0,
		RESTART,
		END,
		MAX
	};
public:	// �p�u���b�N�֐�
	CPause();
	~CPause();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Setter
	void SetIsDeleted();
	
	// Getter
	bool GetIsDeleted() { return m_isDeleted; }
	Status GetStatus() { return m_status; }

private: // �v���C�x�[�g�֐�
private: // �����o�[�ϐ�
	CObject2D* m_bg;
	CMenu* m_menu;
	Status m_status;
	bool m_isDeleted;
};
#endif // !_PAUSE_H_
