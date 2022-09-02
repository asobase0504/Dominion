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
std::vector<std::list<CObject*>> CObject::object = {};
int CObject::numAll = 0;
bool CObject::isStopUpdate = false;

//----------------------------------------
// コンストラクタ
//----------------------------------------
CObject::CObject(TYPE type, int priority) :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0),
	m_isDeleted(false),
	shouldStopAlsoUpdate(false)
{
	m_type = type;

	if (object.size() < priority)
	{
		object.resize(priority);
	}
	object.at(priority - 1).push_back(this);
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
	for (int i = 0;i < object.size();i++)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			if ((*it) != this)
			{
				continue;
			}

			delete this;
			return object.at(i).erase(it);
		}
	}

	assert(false);
	return object.at(0).begin();
}

//----------------------------------------
// 全ての破棄
//----------------------------------------
void CObject::ReleaseAll()
{
	for (int i = (int)object.size() - 1; i >= 0; i--)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end();)
		{
			(*it)->Uninit();

			if (!(*it)->m_isDeleted)
			{
				delete (*it);
				it = object.at(i).erase(it);
				continue;
			}

			it++;
		}
	}
}

//----------------------------------------
// 全ての更新
//----------------------------------------
void CObject::UpdateAll()
{
	for (int i = 0; i < object.size(); i++)
	{
		// 更新処理
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			if (((*it)->shouldStopAlsoUpdate && isStopUpdate) || !isStopUpdate)
			{
				(*it)->Update();
			}
		}

		// 死亡処理
		for (auto it = object.at(i).begin(); it != object.at(i).end();)
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
}

//----------------------------------------
// 全ての描画
//----------------------------------------
void CObject::DrawAll()
{
	for (int i = 0; i < object.size(); i++)
	{
		for (auto it = object.at(i).begin(); it != object.at(i).end(); it++)
		{
			(*it)->Draw();
		}
	}
}
