//=============================================================================
// 
// �X�^�[�g���̃J�E���g�_�E�����s���N���X
// Author YudaKaito
// 
//=============================================================================
#ifndef _UI_OBTAINED_SETNUMBER_H_
#define _UI_OBTAINED_SETNUMBER_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------------------------------------------
// ���j���[�N���X
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ���j���[�N���X
//-----------------------------------------------------------------------------
class CObtainedSetNumberUI
{
public: // ��`
	static const int PRIORITY_BELONG;	// ���̃N���X�̃v���C�I���e�B�B
	static const int NEW_SCORE_END_IN_TIME;		// �V�K���_�̏o�����I��鎞��
	static const int NEW_SCORE_BEGIN_OUT_TIME;	// �V�K���_�̏������n�܂鎞��
	static const int NEW_SCORE_END_OUT_TIME;	// �V�K���_�̏������I��鎞��
	static const int NEXT_STAGE_TIME;	// ���̃X�e�[�W�Ɉڍs����
public: // �񋓌^

public:	// �ÓI�p�u���b�N�֐�
	static CObtainedSetNumberUI* Create(const D3DXVECTOR2& inPos);
public:	// ���I�p�u���b�N�֐�
	CObtainedSetNumberUI();
	~CObtainedSetNumberUI();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Setter
	void SetIsDeleted();

	// Getter
	bool GetIsDeleted() { return m_isDeleted; }

private: // ���I�v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	int m_time;
	D3DXVECTOR2 m_pos;	// �ʒu
	CObject2D* m_fream;	// �t���[��
	std::vector<std::vector<CObject2D*>> m_tookSetUI;	// �擾�����Z�b�g��UI
	std::vector<std::vector<CObject2D*>> m_setCountUI;	// �Z�b�g��UI
	bool m_isDeleted;
};

#endif // !_UI_OBTAINED_SETNUMBER_H_