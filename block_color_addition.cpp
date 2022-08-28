#include "block_color_addition.h"
#include "application.h"
#include "renderer.h"
#include "map.h"

const int CBlockColorAddition::PRIORITY_BELONG = 2;
const float CBlockColorAddition::SUBTRACT_SPEED = 0.005f;

CBlockColorAddition::CBlockColorAddition(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG)
{

}

CBlockColorAddition::~CBlockColorAddition()
{
}

HRESULT CBlockColorAddition::Init()
{
	CObject2D::Init();
	return S_OK;
}

void CBlockColorAddition::Uninit()
{
	CObject2D::Uninit();
	m_parent->DeleteColorAddition();
}

void CBlockColorAddition::Update()
{
	CObject2D::Update();

	m_col.a -= SUBTRACT_SPEED;

	if (m_col.a <= 0.0f)
	{
		SetIsDeleted(true);
	}
	SetColor(m_col);
}

void CBlockColorAddition::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

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
	col.a = 0.5f;
	colorAddition->SetColor(col);

	return colorAddition;
}
