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
class CEndGameUI;
class CMenu;
class CPause;

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

	// 更新処理
	void Update() override;
	void BattleEnd();

	void Draw() override;

	// Getter
	CStage* GetStage() { return m_stage; }	// ステージ取得
	nlohmann::json GetStageInfo() { return m_stageInfo[m_stageIndex]; }	// ステージ情報取得
	int GetNeedWinNumber() { return m_needWinNumber; }	// ラウンド数取得
	int GetWinNumber(int inIndex) { return m_winNumber[inIndex]; }	// 勝利数取得
	int GetWinner() { return m_winnerIndex; }						// 勝利したチーム番号取得

private: // 動的プライベート関数

	/* ステージ選択系統 */
	void StageSelectInit();
	void StageSelectUpdate();

	/* 人数選択系統 */
	void PeopleNumberSelectInit();
	void PeopleNumberSelectUpdate();
	void PeopleNumberSelectDelete();
	void ResetCharcterSample();

	/* キャラクター選択系統 */
	void CharctorSelect();
	void CharcterSelectInit();
	void SetControllerIndex();

	void BattleUpdate();
	void ResetStage();

private: // 動的メンバー変数

	CPause* m_pause;

	int m_updateLagTime;
	std::vector<int> m_controllerIndex;	// コントローラーの番号
	int m_peopleNumber;				// 今回の人数

	/* ステージ選択系統 */
	CMenu* m_stageSelect;		// ステージ選択
	nlohmann::json m_stageInfo;	// ステージ情報の集まり
	int m_stageIndex;			// ステージ番号
	CStage* m_stage;			// ステージ

	/* 人数選択UI系統 */
	CMenu* m_peopleNumberSelect;	// 人数選択
	std::vector<CObject2D*> m_charcterBg;	// キャラクターを置く位置の背景
	std::vector<CObject2D*> m_charcter;		// キャラクターの画像
	std::vector<CObject2D*> m_bulletSample;	// キャラクター弾数の画像

	/* キャラクター選択UI系統 */
	std::vector<int> m_contrellerTeam;				// コントローラーが所属する予定のチーム
	std::vector<CObject2D*> m_contrellerTexture;	// コントローラーのテクスチャ
	std::vector<D3DXVECTOR3> m_contrellerPos;		// コントローラーが何も選択してない時の位置
	CObject2D* m_contrellerTextureBg; // コントローラーのテクスチャを置くは背景

	/* 戦闘系統 */
	// UI関係
	CCountDownUI* m_countDownUI;					// カウントダウンUI
	CObtainedSetNumberUI* m_obtainedSetNumberUI;	// ラウンド終了時のUI
	CEndGameUI* m_endGameUI;						// エンドゲームUI

	// ラウンド勝利関係
	int m_winnerIndex;				// ラウンド勝利チームの番号
	std::vector<int> m_winNumber;	// 現在ラウンド勝利数
	int m_needWinNumber;			// 必要ラウンド勝利数
};

#endif // !_GAME_H_
