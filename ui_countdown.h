//=========================================
// 
// スタート時のカウントダウンを行うクラス
// Author YudaKaito
// 
//=========================================
#ifndef _UI_COUNTDOWN_H_
#define _UI_COUNTDOWN_H_

#include "object2d.h"

//-----------------------------------------
// メニュークラス
//-----------------------------------------
class CCountDownUI
{
public: // 定義
	static const int READY_TIME;		// スタートまでの時間
	static const int GO_TIME;		// スタートまでの時間
	static const int PRIORITY_BELONG;	// このクラスのプライオリティ。
public: // 列挙型

public:	// 静的パブリック関数
	static CCountDownUI* Create(const D3DXVECTOR2& inPos);
public:	// 動的パブリック関数
	CCountDownUI();
	~CCountDownUI();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetIsDeleted() { return m_isDeleted; }

private: // 動的プライベート関数
	void SetIsDeleted();

private: // メンバー変数
	D3DXVECTOR2 m_pos;	// 位置
	CObject2D* m_fream;
	CObject2D* m_text;
	int m_count;	// ゲームスタートまでのカウントタイマー
	bool m_isDeleted;
};

#endif // !_UI_COUNTDOWN_H_