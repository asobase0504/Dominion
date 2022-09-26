//=========================================
// 
// �X�e�[�W�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "mode.h"
#include "file.h"
#include <d3dx9.h>
#include <vector>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CCharacter;
class CController;
class CMap;
class CCountDownUI;

//-----------------------------------------
// �X�e�[�W�N���X
//-----------------------------------------
class CStage
{
public:	// �����o�[�֐�
	CStage();
	~CStage();

	HRESULT Init(nlohmann::json inStage);
	void Uninit();
	void Update();
	void Draw();
	void AllDelete();

	// Setter
	void SetControllerIndex(const std::vector<int>& inControllerIndex) { m_controllerIndex = inControllerIndex; }

	// Getter
	bool GetEndSet() { return m_isEndGame; }
	int GetWinnerIndex();
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // �v���C�x�[�g�֐�
	void PassOnceCreatePlater();
	void DeleteBullet();
private: // ���I�����o�[�ϐ�

	std::vector<CCharacter*> character;	// �L�����N�^�[
	std::vector<int> m_controllerIndex;
	std::vector<CController*> controller;	// �R���g���[���[
	nlohmann::json stage;	// �X�e�[�W�̏��
	CMap* map;			// �}�b�v�f�[�^
	int m_winnarTeam;	// �������`�[��
	bool m_isPopFlag;	// �L�����N�^�[�̏o���𐧌䂷��t���O
	int m_PreparingCount;	// ���������܂ł�
	bool m_isEndGame;	// ���E���h�I������
};

#endif // !_GAME_H_
