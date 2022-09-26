//=========================================
// 
// �`���[�g���A�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "mode.h"
#include "file.h"
#include <d3dx9.h>
#include <vector>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CStage;
class CCharacter;
class CController;
class CMap;
class CCountDownUI;
class CObtainedSetNumberUI;
class CEndGameUI;
class CMenu;
class CPause;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CTutorial : public CMode
{
public:	// �����o�[�֐�
	CTutorial();
	~CTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetControllerIndex(const int inControllerIndex);

private: // ���I�v���C�x�[�g�֐�
	void ResetStage();

private: // ���I�����o�[�ϐ�

	/* �X�e�[�W�I���n�� */
	nlohmann::json m_stageInfo;	// �X�e�[�W���̏W�܂�
	int m_stageIndex;			// �X�e�[�W�ԍ�
	CStage* m_stage;			// �X�e�[�W

	/* �l���I���n�� */
	CMenu* m_peopleNumberSelect;	// �l���I��
	int m_peopleNumber;				// ����̐l��

	/* �L�����N�^�[�I���n�� */
	CMenu* m_charcterSelect;			// �L�����N�^�[�I��
};

#endif // !_GAME_H_
