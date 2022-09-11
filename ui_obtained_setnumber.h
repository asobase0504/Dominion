//=============================================================================
// 
// スタート時のカウントダウンを行うクラス
// Author YudaKaito
// 
//=============================================================================
#ifndef _UI_OBTAINED_SETNUMBER_H_
#define _UI_OBTAINED_SETNUMBER_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------------------------------------------
// メニュークラス
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// メニュークラス
//-----------------------------------------------------------------------------
class CObtainedSetNumberUI
{
public: // 定義
	static const int PRIORITY_BELONG;	// このクラスのプライオリティ。
	static const int NEW_SCORE_END_IN_TIME;		// 新規得点の出現が終わる時間
	static const int NEW_SCORE_BEGIN_OUT_TIME;	// 新規得点の消失が始まる時間
	static const int NEW_SCORE_END_OUT_TIME;	// 新規得点の消失が終わる時間
	static const int NEXT_STAGE_TIME;	// 次のステージに移行時間
public: // 列挙型

public:	// 静的パブリック関数
	static CObtainedSetNumberUI* Create(const D3DXVECTOR2& inPos);
public:	// 動的パブリック関数
	CObtainedSetNumberUI();
	~CObtainedSetNumberUI();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Setter
	void SetIsDeleted();

	// Getter
	bool GetIsDeleted() { return m_isDeleted; }

private: // 動的プライベート関数

private: // メンバー変数
	int m_time;
	D3DXVECTOR2 m_pos;	// 位置
	CObject2D* m_fream;	// フレーム
	std::vector<std::vector<CObject2D*>> m_tookSetUI;	// 取得したセット数UI
	std::vector<std::vector<CObject2D*>> m_setCountUI;	// セット数UI
	bool m_isDeleted;
};

#endif // !_UI_OBTAINED_SETNUMBER_H_