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
class CMenu;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CGame : public CMode
{
public:	// メンバー関数
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void StageSelectInit();
	void Uninit() override;
	void Update() override;
	void StageSelectUpdate();
	void PeopleNumberSelectUpdate();
	void CharctorSelect();
	void BattleUpdate();
	void BattleEnd();

	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// ステージの情報
	nlohmann::json GetStageInfo() { return stageInfo[m_stageIndex]; }
	std::vector<int> GetControllerIndex() { return m_controllerIndex; }	// コントローラー番号取得
	int GetNeedWinNumber() { return m_needWinNumber; }	// ラウンド数の取得
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// 勝利数の取得
	int GetWinner() { return m_winnerIndex; }						// 勝利したチームの番号取得

private: // 動的プライベート関数
	void ResetStage();

private: // 動的メンバー変数

	/* ステージ選択系統 */
	CMenu* m_stageSelect;	// ステージ選択
	nlohmann::json stageInfo;	// ステージ情報の集まり
	int m_stageIndex;		// ステージ番号
	CStage* m_stage;		// ステージ

	/* 人数選択系統 */
	CMenu* m_peopleNumberSelect;	// 人数選択
	int m_peopleNumber;				// 今回の人数
	
	/* キャラクター選択系統 */
	CMenu* m_charcterSelect;		// キャラクター選択
	std::vector<int> m_controllerIndex;	// コントローラーの番号

	/* 戦闘系統 */
	// UI関係
	CCountDownUI* m_countDownUI;	// カウントダウンUI
	CObtainedSetNumberUI* m_obtainedSetNumberUI;	// ラウンド終了時のUI

	// ラウンド勝利関係
	int m_winnerIndex;				// ラウンド勝利チームの番号
	std::vector<int> m_winNumber;	// 現在ラウンド勝利数
	int m_needWinNumber;			// 必要ラウンド勝利数
};

#endif // !_GAME_H_
