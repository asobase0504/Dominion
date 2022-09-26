#include "block_color_addition.h"
#include "application.h"
#include "renderer.h"
#include "map.h"
#include "stage.h"

const int CBlockColorAddition::PRIORITY_BELONG = 2;
const float CBlockColorAddition::SUBTRACT_SPEED = 0.005f;
const float CBlockColorAddition::BIGIN_ALPHA = 0.5f;

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CBlockColorAddition::CBlockColorAddition(CObject::TYPE type) :
	CObject2D(type, PRIORITY_BELONG)
{

}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CBlockColorAddition::~CBlockColorAddition()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CBlockColorAddition::Init()
{
	CObject2D::Init();
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CBlockColorAddition::Uninit()
{
	CObject2D::Uninit();
	//m_parent->DeleteColorAddition();
}

//--------------------------------------------------
// �X�V
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
// �`��
//--------------------------------------------------
void CBlockColorAddition::Draw()
{
	CObject2D::Draw();
}

//--------------------------------------------------
// �쐬
//--------------------------------------------------
CBlockColorAddition * CBlockColorAddition::Create(const D3DXVECTOR3 & inPos, CBlock* inParent,CMap* inMap)
{
	CBlockColorAddition* colorAddition = new CBlockColorAddition;

	if (colorAddition == nullptr)
	{
		return nullptr;
	}

	colorAddition->m_parent = inParent;
	colorAddition->Init();
	colorAddition->SetPos(inPos);

	colorAddition->SetSize(D3DXVECTOR2(inMap->GetBlockSize(), inMap->GetBlockSize()));
	D3DXCOLOR col(CApplication::GetInstance()->GetColor(2));
	col.a = BIGIN_ALPHA;
	colorAddition->SetColor(col);

	return colorAddition;
}
