//=========================================
// 
// ステージクラス
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
// 前方宣言
//-----------------------------------------
class CCharacter;
class CController;
class CMap;
class CCountDownUI;

//-----------------------------------------
// ステージクラス
//-----------------------------------------
class CStage
{
public:	// メンバー関数
	CStage();
	~CStage();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// ゲッタ―
	bool GetEndSet() { return m_isEndGame; }
	CMap* GetMap() { return map; }
	std::vector<CCharacter*>* GetCharacter() { return &character; }

private: // プライベート関数
	void BulletDelete();

private: // 動的メンバー変数
	std::vector<CCharacter*> character;
	std::vector<CController*> controller;
	nlohmann::json stage;
	CMap* map;
	bool m_isEndGame;
};

#endif // !_GAME_H_
