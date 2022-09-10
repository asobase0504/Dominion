//=========================================
// 
// ゲーム内クラス
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
// 前方宣言
//-----------------------------------------
class CStage;
class CCharacter;
class CController;
class CMap;
class CCountDownUI;
class CObtainedSetNumberUI;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CGame : public CMode
{
public:	// メンバー関数
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// ステージの情報
	int GetNeedWinNumber() { return m_needWinNumber; }	// ラウンド数の取得
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// 勝利数の取得
	int GetWinner() { return m_winnerIndex; }

private: // 動的プライベート関数
	void ResetStage();

private: // 動的メンバー変数
	CStage* m_stage;	// ステージ
	CCountDownUI* m_countDownUI;	// カウントダウンUI
	CObtainedSetNumberUI* m_obtainedSetNumberUI;	// ラウンド終了時のUI
	int m_winnerIndex;
	std::vector<int> m_winNumber;	// 現在勝利数
	int m_needWinNumber;			// 必要勝利数
};

#endif // !_GAME_H_
