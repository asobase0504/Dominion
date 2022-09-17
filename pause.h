//=========================================
// 
// ポーズ中クラス
// Author YudaKaito
// 
//=========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include <Windows.h>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CObject2D;
class CMenu;

//-----------------------------------------
// メニュークラス
//-----------------------------------------
class CPause
{
public: // 列挙型
	enum Status
	{
		BACK = 0,
		RESTART,
		END,
		MAX
	};
public:	// パブリック関数
	CPause();
	~CPause();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Setter
	void SetIsDeleted();
	
	// Getter
	bool GetIsDeleted() { return m_isDeleted; }
	Status GetStatus() { return m_status; }

private: // プライベート関数
private: // メンバー変数
	CObject2D* m_bg;
	CMenu* m_menu;
	Status m_status;
	bool m_isDeleted;
};
#endif // !_PAUSE_H_
