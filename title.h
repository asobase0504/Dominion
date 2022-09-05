//=========================================
// 
// タイトル内クラス
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include <d3dx9.h>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CMenu;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CTitle : public CMode
{
public: // 列挙型
	enum class Status
	{
		GAME_STAET,
		TUTORIAL,
		CUSTOMIZE,
		OPSITON,
		EXIT,
		MAX,
		NONE
	};
public:	// メンバー関数
	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // 動的メンバー変数
	CMenu* m_manu;
	Status m_status;
};

#endif // !_TITLE_H_
