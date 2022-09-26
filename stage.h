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

private: // プライベート関数
	void PassOnceCreatePlater();
	void DeleteBullet();
private: // 動的メンバー変数

	std::vector<CCharacter*> character;	// キャラクター
	std::vector<int> m_controllerIndex;
	std::vector<CController*> controller;	// コントローラー
	nlohmann::json stage;	// ステージの情報
	CMap* map;			// マップデータ
	int m_winnarTeam;	// 勝ったチーム
	bool m_isPopFlag;	// キャラクターの出現を制御するフラグ
	int m_PreparingCount;	// 準備完了までの
	bool m_isEndGame;	// ラウンド終了中か
};

#endif // !_GAME_H_
