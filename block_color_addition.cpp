#include "block_color_addition.h"
#include "application.h"
#include "renderer.h"
#include "map.h"

const int CBlockColorAddition::PRIORITY_BELONG = 2;
const float CBlockColorAddition::SUBTRACT_SPEED = 0.005f;
const float CBlockColorAddition::BIGIN_ALPHA = 0.5f;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBlockColorAddition::CBlockColorAddition(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG)
{

}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CBlockColorAddition::~CBlockColorAddition()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CBlockColorAddition::Init()
{
	CObject2D::Init();
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CBlockColorAddition::Uninit()
{
	CObject2D::Uninit();
	//m_parent->DeleteColorAddition();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBlockColorAddition::Update()
{
	CObject2D::Update();

	m_col.a -= SUBTRACT_SPEED;

	SetColor(m_col);

	if (m_col.a <= 0.0f)
	{
		SetIsDeleted(true);
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CBlockColorAddition::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------------------
// 作成
//--------------------------------------------------
CBlockColorAddition * CBlockColorAddition::Create(const D3DXVECTOR3 & inPos, CBlock* inParent)
{
	CBlockColorAddition* colorAddition = new CBlockColorAddition;

	if (colorAddition == nullptr)
	{
		return nullptr;
	}

	colorAddition->m_parent = inParent;
	colorAddition->Init();
	colorAddition->SetPos(inPos);
	colorAddition->SetSize(D3DXVECTOR2(CMap::GetBlockSize(), CMap::GetBlockSize()));
	D3DXCOLOR col(CApplication::GetInstance()->GetColor(2));
	col.a = BIGIN_ALPHA;
	colorAddition->SetColor(col);

	return colorAddition;
}
