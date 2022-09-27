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
	void Uninit() override;

	// �X�V����
	void Update() override;
	void BattleEnd();

	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// �X�e�[�W�擾
	nlohmann::json GetStageInfo() { return m_stageInfo[m_stageIndex]; }	// �X�e�[�W���擾
	int GetNeedWinNumber() { return m_needWinNumber; }	// ���E���h���擾
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// �������擾
	int GetWinner() { return m_winnerIndex; }						// ���������`�[���ԍ��擾

private: // ���I�v���C�x�[�g�֐�

	/* �X�e�[�W�I���n�� */
	void StageSelectInit();
	void StageSelectUpdate();

	/* �l���I���n�� */
	void PeopleNumberSelectInit();
	void PeopleNumberSelectUpdate();
	void PeopleNumberSelectDelete();
	void ResetCharcterSample();

	/* �L�����N�^�[�I���n�� */
	void CharctorSelect();
	void CharcterSelectInit();
	void SetControllerIndex();

	void BattleUpdate();
	void ResetStage();

private: // ���I�����o�[�ϐ�

	CPause* m_pause;

	int m_updateLagTime;
	std::vector<int> m_controllerIndex;	// �R���g���[���[�̔ԍ�
	int m_peopleNumber;				// ����̐l��

	/* �X�e�[�W�I���n�� */
	CMenu* m_stageSelect;		// �X�e�[�W�I��
	nlohmann::json m_stageInfo;	// �X�e�[�W���̏W�܂�
	int m_stageIndex;			// �X�e�[�W�ԍ�
	CStage* m_stage;			// �X�e�[�W

	/* �l���I��UI�n�� */
	CMenu* m_peopleNumberSelect;	// �l���I��
	std::vector<CObject2D*> m_charcterBg;	// �L�����N�^�[��u���ʒu�̔w�i
	std::vector<CObject2D*> m_charcter;		// �L�����N�^�[�̉摜
	std::vector<CObject2D*> m_bulletSample;	// �L�����N�^�[�e���̉摜

	/* �L�����N�^�[�I��UI�n�� */
	std::vector<int> m_contrellerTeam;				// �R���g���[���[����������\��̃`�[��
	std::vector<CObject2D*> m_contrellerTexture;	// �R���g���[���[�̃e�N�X�`��
	std::vector<D3DXVECTOR3> m_contrellerPos;		// �R���g���[���[�������I�����ĂȂ����̈ʒu
	CObject2D* m_contrellerTextureBg; // �R���g���[���[�̃e�N�X�`����u���͔w�i

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
