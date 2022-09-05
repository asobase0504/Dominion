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

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �Q�b�^�\
	bool GetEndSet() { return m_isEndGame; }
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // �v���C�x�[�g�֐�
	void BulletDelete();

private: // ���I�����o�[�ϐ�
	std::vector<CCharacter*> character;
	std::vector<CController*> controller;
	nlohmann::json stage;
	CMap* map;
	bool m_isEndGame;
};

#endif // !_GAME_H_
