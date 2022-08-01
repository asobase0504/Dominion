//=========================================
// 
// �Q�[�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

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
class CGame
{
public:	// �����o�[�֐�
	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �Q�b�^�\
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // ���I�����o�[�ϐ�
	std::vector<CCharacter*> character;
	CMap* map;
};

#endif // !_GAME_H_
