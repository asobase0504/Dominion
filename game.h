//=========================================
// 
// �Q�[�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

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
class CGame : public CMode
{
public:	// �����o�[�֐�
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void StageSelectInit();
	void PeopleNumberSelectInit();
	void CharcterSelectInit();
	void Uninit() override;

	// �X�V����
	void Update() override;
	void StageSelectUpdate();
	void PeopleNumberSelectUpdate();

	void CharctorSelect();
	void BattleUpdate();
	void BattleEnd();

	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// �X�e�[�W�擾
	nlohmann::json GetStageInfo() { return m_stageInfo[m_stageIndex]; }	// �X�e�[�W���擾
	std::vector<int> GetControllerIndex() { return m_controllerIndex; }	// �R���g���[���[�ԍ��擾
	int GetNeedWinNumber() { return m_needWinNumber; }	// ���E���h���擾
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// �������擾
	int GetWinner() { return m_winnerIndex; }						// ���������`�[���ԍ��擾

private: // ���I�v���C�x�[�g�֐�
	void ResetStage();

private: // ���I�����o�[�ϐ�

	CPause* m_pause;

	int m_updateLagTime;

	/* �X�e�[�W�I���n�� */
	CMenu* m_stageSelect;		// �X�e�[�W�I��
	nlohmann::json m_stageInfo;	// �X�e�[�W���̏W�܂�
	int m_stageIndex;			// �X�e�[�W�ԍ�
	CStage* m_stage;			// �X�e�[�W

	/* �l���I���n�� */
	CMenu* m_peopleNumberSelect;	// �l���I��
	int m_peopleNumber;				// ����̐l��
	
	/* �L�����N�^�[�I���n�� */
	CMenu* m_charcterSelect;			// �L�����N�^�[�I��
	std::vector<int> m_controllerIndex;	// �R���g���[���[�̔ԍ�

	/* �퓬�n�� */
	// UI�֌W
	CCountDownUI* m_countDownUI;					// �J�E���g�_�E��UI
	CObtainedSetNumberUI* m_obtainedSetNumberUI;	// ���E���h�I������UI
	CEndGameUI* m_endGameUI;						// �G���h�Q�[��UI

	// ���E���h�����֌W
	int m_winnerIndex;				// ���E���h�����`�[���̔ԍ�
	std::vector<int> m_winNumber;	// ���݃��E���h������
	int m_needWinNumber;			// �K�v���E���h������
};

#endif // !_GAME_H_
