//=========================================
// 
// メニュークラス
// Author YudaKaito
// 
//=========================================
#ifndef _MENU_H_
#define _MENU_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CMenuFream;
class CMenuItem;

//-----------------------------------------
// メニュークラス
//-----------------------------------------
class CMenu
{
public: // 定義
	static const int DECISION_AFTERGLOW_TIME;	// 決定後の余韻時間
public: // 列挙型
	enum SELECT_DIRECTION
	{
		TOP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

public:	// パブリック関数
	CMenu();
	~CMenu();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Select(SELECT_DIRECTION inDirection);
	bool Decision(bool inDecison);
	virtual void UpdateBeforeDecision();
	virtual void UpdateAfterDecision();

	// 選択項目の追加
	void AddItemX(CMenuItem* inItem , int indexY = 0);
	void AddItemY(CMenuItem* inItems,int indexX = 0);
	void AddItemY(std::vector<CMenuItem*> inItems);

	// Setter
	void SetSelectIdx(int Y, int X);
	void SetSelectIdx(std::vector<int> inIdx);
	void SetInterval(D3DXVECTOR2 inIntervar);
	void SetInterval(float inIntervarX, float inIntervarY);
	void SetIsDeleted();
	void SetItems(const std::vector<std::vector<CMenuItem*>>& inItems);

	// Getter
	const std::vector<int>& GetSelectIdx() { return m_selectIdx; }
	CMenuFream* GetFream() { return m_fream; }

	static CMenu* Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea,CMenuFream* inFream);
protected: // プライベート関数
	// Setter
	void SetItemPos();
protected: // メンバー変数
	D3DXVECTOR2 m_pos;	// 位置
	D3DXVECTOR2 m_Area;	// 範囲
	CMenuFream* m_fream;	// フレーム
	std::vector<std::vector<CMenuItem*>> m_item;	// 項目
	std::vector<int> m_selectIdx;					// 現在選択中の番号
	D3DXVECTOR2 m_fInterval;						// 項目の間隔
	D3DXVECTOR2 m_AroundWhitespace;					// 周りの余白
	bool m_isDexision;	// 決定しているか否か
	int m_decisionAfterglowCount;					// 決定後の余韻カウント
};

//-----------------------------------------
// フレームクラス
//-----------------------------------------
class CMenuFream : public CObject2D
{
public:	// パブリック関数
	CMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CMenuFream() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

private: // プライベート関数

private: // メンバー変数

};

//-----------------------------------------
// 基本項目クラス
//-----------------------------------------
class CMenuItem : public CObject2D
{
public:	// パブリック関数
	CMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CMenuItem() override;

	virtual HRESULT Init() override;

	virtual void PopUpdate();
	virtual void SelectUpdate();
	virtual void NomalUpdate();
	virtual void EndUpdate();

	// Setter
	void SetPopDuringUpdateAccept(bool inAccept) { m_isPopDuringUpdateAccept = inAccept; }

	// Getter
	bool GetisPopNow() { return m_isPopNow; }

private: // プライベート関数

protected: // メンバー変数
	bool m_isPopDuringUpdateAccept;	// ポップ中他のアップデートを受け入れるか
	bool m_isPopNow;	// 現在ポップ中か
};

#endif // !_CHARACTER_H_
