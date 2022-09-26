//=========================================
// 
// チュートリアル内クラス
// Author YudaKaito
// 
//=========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

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
class CTutorial : public CMode
{
public:	// メンバー関数
	CTutorial();
	~CTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetControllerIndex(const int inControllerIndex);

private: // 動的プライベート関数
	void ResetStage();

private: // 動的メンバー変数

	/* ステージ選択系統 */
	nlohmann::json m_stageInfo;	// ステージ情報の集まり
	int m_stageIndex;			// ステージ番号
	CStage* m_stage;			// ステージ

	/* 人数選択系統 */
	CMenu* m_peopleNumberSelect;	// 人数選択
	int m_peopleNumber;				// 今回の人数

	/* キャラクター選択系統 */
	CMenu* m_charcterSelect;			// キャラクター選択
};

#endif // !_GAME_H_
