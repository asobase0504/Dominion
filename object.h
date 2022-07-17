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

class CObject
{
public:	// 列挙型
	// CObjectクラスの派生先
	enum TYPE
	{
		OBJECT_2D
	};

public:	// 定数
	static const int NUM_MAX = 1500;

public:	// 静的関数
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

public:
	CObject();
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	const D3DXVECTOR3& GetPos() const { return m_pos; }	// 位置の取得
	virtual void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }	// 位置の設定

private:
	auto Release();

private:
	static std::list<CObject*> object;	// オブジェクト
	static int numAll;	// 最大数
	int createIdx;		// 生成番号

protected:
	D3DXVECTOR3 m_pos;	// 位置
	bool m_enabled;		// 存在してるか？
};

#endif // !_RENDERER_H_
