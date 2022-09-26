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
public:
	enum CONTENT
	{
		NONE = 0,
		MOVE,
		SHOT,
		CHARGE
	};
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
	void ResetText();
	void CheckMission();
private: // 動的メンバー変数

	int m_controllerIndex;

	/* ステージ選択系統 */
	nlohmann::json m_stageInfo;	// ステージ情報の集まり
	int m_stageIndex;			// ステージ番号
	CStage* m_stage;			// ステージ

	/* フォント関係 */
	CONTENT m_content;
	DESIGNVECTOR m_design;
	LPD3DXFONT m_fontSubject;
	std::string m_textSubject;
	LPD3DXFONT m_font;
	std::vector<std::string> m_text;
	std::vector<bool> m_isSuccess;
	bool isResetText;

	int chargeCount;
};

#endif // !_GAME_H_
