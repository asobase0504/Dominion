//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#include "object.h"
#include "renderer.h"

//=========================================
// 静的メンバー変数の宣言
//=========================================
const int CObject::NUM_MAX;
CObject* CObject::object[NUM_MAX] = {};
int CObject::numAll = 0;
 
//----------------------------------------
// コンストラクタ
//----------------------------------------
CObject::CObject() :
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	createIdx(0)
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] == nullptr)
		{
			numAll++;
			object[i] = this;
			createIdx = i;
			break;
		}
	}
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
void CObject::Release()
{
	int idx = createIdx;
	if (object[idx] != nullptr)
	{
		object[idx]->Uninit();
		delete object[idx];
		object[idx] = nullptr;
		numAll--;
	}
}

//----------------------------------------
// 全ての破棄
//----------------------------------------
void CObject::ReleaseAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Uninit();
			delete object[i];
			object[i] = nullptr;
		}
	}
}

//----------------------------------------
// 全ての更新
//----------------------------------------
void CObject::UpdateAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Update();
		}
	}
}

//----------------------------------------
// 全ての描画
//----------------------------------------
void CObject::DrawAll()
{
	for (int i = 0; i < NUM_MAX; i++)
	{
		if (object[i] != nullptr)
		{
			object[i]->Draw();
		}
	}
}
