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

public: // 列挙型

public:	// パブリック関数
	CMenu();
	~CMenu();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CMenu* Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea,CMenuFream* inFream, std::vector<std::vector<CMenuItem*>> inItem);
private: // プライベート関数

private: // メンバー変数
	D3DXVECTOR2 m_pos;	// 位置
	D3DXVECTOR2 m_Area;	// 位置
	CMenuFream* m_fream;	// フレーム
	std::vector<std::vector<CMenuItem*>> m_item;	// 項目
	int m_selectIdx;				// 現在選択中の番号
	D3DXVECTOR2 m_fInterval;		// 項目の間隔
	D3DXVECTOR2 m_AroundWhitespace;	// 周りの余白
};

//-----------------------------------------
// フレームクラス
//-----------------------------------------
class CMenuFream : public CObject2D
{
public:	// パブリック関数
	CMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	~CMenuFream() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // プライベート関数

private: // メンバー変数

};

//-----------------------------------------
// 項目クラス
//-----------------------------------------
class CMenuItem : public CObject2D
{
public:	// パブリック関数
	CMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // プライベート関数

private: // メンバー変数

};

#endif // !_CHARACTER_H_
