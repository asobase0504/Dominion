//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#include "object.h"
#include "renderer.h"

#include <assert.h>

//=========================================
// 静的メンバー変数の宣言
//=========================================
const int CObject::NUM_MAX;
std::list<CObject*> CObject::object = {};
int CObject::numAll = 0;
 
//----------------------------------------
// コンストラクタ
//----------------------------------------
CObject::CObject(TYPE type) :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0),
	m_isDeleted(false)
{
	m_type = type;
	object.push_back(this);
}

//----------------------------------------
// デストラクタ
//----------------------------------------
CObject::~CObject()
{
}

//----------------------------------------
// 破棄
//----------------------------------------
auto CObject::Release()
{
	for (auto it = object.begin(); it != object.end(); it++)
	{
		if ((*it) != this)
		{
			continue;
		}

		delete this;
		return object.erase(it);
	}

	assert(false);
	return object.begin();
}

//----------------------------------------
// 全ての破棄
//----------------------------------------
void CObject::ReleaseAll()
{
	for (auto it = object.begin(); it != object.end();)
	{
		(*it)->Uninit();

		if (!(*it)->m_isDeleted)
		{
			it = (*it)->Release();
			continue;
		}

		it++;
	}
}

//----------------------------------------
// 全ての更新
//----------------------------------------
void CObject::UpdateAll()
{
	for (auto it = object.begin(); it != object.end(); it++)
	{
		(*it)->Update();
	}

	for (auto it = object.begin(); it != object.end();)
	{
		if ((*it)->m_isDeleted)
		{
			(*it)->Uninit();
			it = (*it)->Release();
			continue;
		}

		it++;
	}
}

//----------------------------------------
// 全ての描画
//----------------------------------------
void CObject::DrawAll()
{
	for (auto it = object.begin(); it != object.end(); it++)
	{
		(*it)->Draw();
	}
}
