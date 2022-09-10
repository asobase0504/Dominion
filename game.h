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
	void Update() override;
	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// �X�e�[�W�̏��
	int GetNeedWinNumber() { return m_needWinNumber; }	// ���E���h���̎擾
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// �������̎擾
	int GetWinner() { return m_winnerIndex; }

private: // ���I�v���C�x�[�g�֐�
	void ResetStage();

private: // ���I�����o�[�ϐ�
	CStage* m_stage;	// �X�e�[�W
	CCountDownUI* m_countDownUI;	// �J�E���g�_�E��UI
	CObtainedSetNumberUI* m_obtainedSetNumberUI;	// ���E���h�I������UI
	int m_winnerIndex;
	std::vector<int> m_winNumber;	// ���ݏ�����
	int m_needWinNumber;			// �K�v������
};

#endif // !_GAME_H_
