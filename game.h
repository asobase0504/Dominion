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
class CCharacter;
class CMap;

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

	// �Q�b�^�\
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // ���I�����o�[�ϐ�
	std::vector<CCharacter*> character;
	nlohmann::json stage;
	CMap* map;
};

#endif // !_GAME_H_
