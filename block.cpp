//==================================================
//
// 正方形のポリゴンの生成
// Author : Yuda Kaito
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "block.h"
#include "character.h"
// デバッグ
#include <assert.h>

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type),
	m_team(BLOCK_TYPE::NONE)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CBlock::~CBlock()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
HRESULT CBlock::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_BLOCK);
	return S_OK;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
void CBlock::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBlock::Update()
{
	ResurveyRidingObject();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CBlock::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
CBlock* CBlock::Create(CBlock::BLOCK_TYPE type)
{
	CBlock* block = new CBlock;

	if (block == nullptr)
	{
		return nullptr;
	}

	block->Init();

	block->m_team = type;

	switch (type)
	{
	case BLOCK_TYPE::NONE:
		block->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	case BLOCK_TYPE::BLOCK_01:
		block->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		block->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	default:
		break;
	}

	return block;
}

//--------------------------------------------------
// 種別の切り替え
//--------------------------------------------------
void CBlock::ChangeType()
{
	switch (m_team)
	{
	case BLOCK_TYPE::NONE:
		break;
	case BLOCK_TYPE::BLOCK_01:
		m_team = BLOCK_TYPE::BLOCK_02;
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		break;
	case BLOCK_TYPE::BLOCK_02:
		m_team = BLOCK_TYPE::BLOCK_01;
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	default:
		MessageBox(NULL, TEXT("想定外の列挙型を検出。"), TEXT("swith文の条件式"), MB_ICONHAND);
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 種別の取得
//--------------------------------------------------
CBlock::BLOCK_TYPE CBlock::GetType()
{
	return m_team;
}

//--------------------------------------------------
// 乗ってるオブジェクトがまだ乗ってるか再検査する
//--------------------------------------------------
void CBlock::ResurveyRidingObject()
{
	for (auto it = ridingObject.begin(); it != ridingObject.end();)
	{
		CObject::TYPE type = (*it)->GetType();

		switch (type)
		{
		case CObject::TYPE::CHARACTER:
		{
			CCharacter* character = (CCharacter*)(*it);	// イテレータをキャラクターに変換。

			if (!character->HitWithBlock(this))
			{ // 当たってない場合
				// 乗ってるオブジェクトを動的配列から削除
				it = ridingObject.erase(it);
				continue;
			}
			break;
		}
		case CObject::TYPE::BULLET:
			break;
		default:
			break;
		}

		// 一度も削除されなければ、
		it++;
	}
}

//--------------------------------------------------
// 乗ってるオブジェクトを設定する
//--------------------------------------------------
void CBlock::SetRidingObject(CObject * inObject)
{
	for (const auto& it : ridingObject)
	{
		// 同じオブジェクトがあった場合は弾く。
		if (it == inObject)
		{
			return;
		}
	}
	// 乗ってるオブジェクトを保存する
	ridingObject.push_back(inObject);
}
