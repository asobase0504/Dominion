//=============================================================================
// 
// �X�^�[�g���̃J�E���g�_�E�����s���N���X
// Author YudaKaito
// 
//=============================================================================
#ifndef _UI_END_GAME_H_
#define _UI_END_GAME_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CMenu;

//-----------------------------------------------------------------------------
// �G���h�Q�[��UI
//-----------------------------------------------------------------------------
class CEndGameUI
{
public: // ��`
	static const int PRIORITY_BELONG;	// ���̃N���X�̃v���C�I���e�B�B
	static const int NEW_SCORE_END_IN_TIME;		// �V�K���_�̏o�����I��鎞��
	static const int NEXT_STAGE_TIME;	// ���̃X�e�[�W�Ɉڍs����
public: // �񋓌^

public:	// �ÓI�p�u���b�N�֐�
	static CEndGameUI* Create(const D3DXVECTOR2& inPos);
public:	// ���I�p�u���b�N�֐�
	CEndGameUI();
	~CEndGameUI();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Setter
	void SetIsDeleted();

	// Getter
	bool GetIsDeleted() { return m_isDeleted; }
	CMenu* GetMenu() { return selectMenu; }

private: // ���I�v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	int m_time;
	D3DXVECTOR2 m_pos;	// �ʒu
	CObject2D* m_fream;	// �t���[��
	CMenu* selectMenu;	// �Z���N�g���j���[
	std::vector<std::vector<CObject2D*>> m_tookSetUI;	// �擾�����Z�b�g��UI
	std::vector<std::vector<CObject2D*>> m_setCountUI;	// �Z�b�g��UI
	bool m_isDeleted;	// ���S�\�肩�ۂ�
};

#endif // !_UI_OBTAINED_SETNUMBER_H_