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
public:
	enum CONTENT
	{
		NONE = 0,
		MOVE,
		SHOT,
		CHARGE
	};
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
	void ResetText();
	void CheckMission();
private: // ���I�����o�[�ϐ�

	int m_controllerIndex;

	/* �X�e�[�W�I���n�� */
	nlohmann::json m_stageInfo;	// �X�e�[�W���̏W�܂�
	int m_stageIndex;			// �X�e�[�W�ԍ�
	CStage* m_stage;			// �X�e�[�W

	/* �t�H���g�֌W */
	CONTENT m_content;
	DESIGNVECTOR m_design;
	LPD3DXFONT m_fontSubject;
	std::string m_textSubject;
	LPD3DXFONT m_font;
	std::vector<std::string> m_text;
	std::vector<bool> m_isSuccess;
	bool isResetText;

	int chargeCount;
};

#endif // !_GAME_H_
