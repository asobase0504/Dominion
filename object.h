//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <d3dx9.h>
#include <list>
#include <vector>

class CObject
{
public:	// 列挙型
	// CObjectクラスの派生先
	enum class TYPE
	{
		CHARACTER = 0,
		BULLET,
		BLOCK,
		NONE
	};

public:	// 定数
	static const int NUM_MAX = 1500;

public:	// 静的関数
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();
	static const std::list<CObject*>* GetMyObject(int inIndex) { return &object.at(inIndex); }
	static int GetPrioritySize() { return (int)object.size(); }
public:
	CObject(TYPE type,int priority = 3);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// Setter
	void SetIsDeleted(bool inDeleted) { m_isDeleted = inDeleted; }
	virtual void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }	// 位置の設定

	// Getter
	const D3DXVECTOR3& GetPos() { return m_pos; }	// 位置の取得
	const TYPE GetType() { return m_type; }
	const bool GetIsDeleted() { return m_isDeleted; }

private:
	auto Release();

private:
	static std::vector<std::list<CObject*>> object;	// オブジェクト
	static int numAll;	// 最大数
	int createIdx;		// 生成番号

protected:
	D3DXVECTOR3 m_pos;	// 位置
	bool m_isDeleted;	// 削除フラグ
	TYPE m_type;		// 種類
};

#endif // !_RENDERER_H_
