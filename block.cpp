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
#include "bullet.h"
#include "application.h"
#include "block_scraped.h"
#include "block_color_addition.h"

// デバッグ
#include <assert.h>

const int CBlock::PRIORITY_BELONG = 1;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBlock::CBlock(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG),
	m_team(CBlock::BLOCK_TYPE::NONE),
	m_scraped(nullptr),
	m_colorAddition(nullptr)
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
	SetTexture("BLOCK");
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CBlock::Uninit()
{
	if (m_colorAddition != nullptr)
	{
		m_colorAddition->Uninit();
		m_colorAddition = nullptr;
	}
	if (m_scraped != nullptr)
	{
		m_scraped->Uninit();
		delete m_scraped;
		m_scraped = nullptr;
	}

	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBlock::Update()
{
	if (m_scraped != nullptr)
	{
		m_scraped->Update();
	}

	if (m_colorAddition != nullptr)
	{
		m_colorAddition->Update();
		if (m_colorAddition->GetIsDeleted())
		{
			m_colorAddition = nullptr;
		}
	}

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
	case CBlock::BLOCK_TYPE::NONE:
		block->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		break;
	case CBlock::BLOCK_TYPE::BLOCK_01:
		block->SetColor(CApplication::GetInstance()->GetColor(0));
		break;
	case CBlock::BLOCK_TYPE::BLOCK_02:
		block->SetColor(CApplication::GetInstance()->GetColor(1));
		break;
	default:
		break;
	}

	return block;
}

//--------------------------------------------------
// 種別の切り替え
//--------------------------------------------------
void CBlock::ChangeType(DIRECTION inDirection)
{
	switch (m_team)
	{
	case CBlock::BLOCK_TYPE::NONE:
		break;
	case CBlock::BLOCK_TYPE::BLOCK_01:
		m_team = CBlock::BLOCK_TYPE::BLOCK_02;
		SetColor(CApplication::GetInstance()->GetColor(1));

		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos,CBlock::BLOCK_01, inDirection);
		break;
	case CBlock::BLOCK_TYPE::BLOCK_02:
		m_team = CBlock::BLOCK_TYPE::BLOCK_01;
		SetColor(CApplication::GetInstance()->GetColor(0));
		if (m_scraped != nullptr)
		{
			m_scraped->SetIsDeleted();
			m_scraped->Uninit();
			delete m_scraped;
			m_scraped = nullptr;
		}
		m_scraped = CBlockScraped::Create(m_pos, CBlock::BLOCK_02, inDirection);
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
		}
		break;
		case CObject::TYPE::BULLET:
		{
			CBullet* bullet = (CBullet*)(*it);	// イテレータを弾に変換。

			if (!bullet->HitWithBlock(this))
			{ // 当たってない場合
			  // 乗ってるオブジェクトを動的配列から削除
				it = ridingObject.erase(it);
				continue;
			}
		}
			break;
		default:
			break;
		}

		// 削除しない場合
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

void CBlock::SetAdditionColor()
{
	if (m_colorAddition != nullptr)
	{
		return;
	}

	m_colorAddition = CBlockColorAddition::Create(m_pos, this);
}
