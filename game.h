//=========================================
// 
// ゲーム内クラス
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

#include <d3dx9.h>
#include <vector>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CCharacter;
class CMap;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CGame
{
public:	// メンバー関数
	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// ゲッタ―
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // 動的メンバー変数
	std::vector<CCharacter*> character;
	CMap* map;
};

#endif // !_GAME_H_
