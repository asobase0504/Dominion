//=========================================
// 
// �X�^�[�g���̃J�E���g�_�E�����s���N���X
// Author YudaKaito
// 
//=========================================
#ifndef _UI_COUNTDOWN_H_
#define _UI_COUNTDOWN_H_

#include "object2d.h"

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CCountDownUI
{
public: // ��`
	static const int READY_TIME;		// �X�^�[�g�܂ł̎���
	static const int GO_TIME;		// �X�^�[�g�܂ł̎���
	static const int PRIORITY_BELONG;	// ���̃N���X�̃v���C�I���e�B�B
public: // �񋓌^

public:	// �ÓI�p�u���b�N�֐�
	static CCountDownUI* Create(const D3DXVECTOR2& inPos);
public:	// ���I�p�u���b�N�֐�
	CCountDownUI();
	~CCountDownUI();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetIsDeleted() { return m_isDeleted; }

private: // ���I�v���C�x�[�g�֐�
	void SetIsDeleted();

private: // �����o�[�ϐ�
	D3DXVECTOR2 m_pos;	// �ʒu
	CObject2D* m_fream;
	CObject2D* m_text;
	int m_count;	// �Q�[���X�^�[�g�܂ł̃J�E���g�^�C�}�[
	bool m_isDeleted;
};

#endif // !_UI_COUNTDOWN_H_