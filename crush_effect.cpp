//=========================================
// 
// 砕けるエフェクト
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "crush_effect.h"
#include "application.h"

//-----------------------------------------
// 定義
//-----------------------------------------

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCrushEffect::CCrushEffect(CObject::TYPE type) :
	CObject2D(type, 2),
	m_life(0)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CCrushEffect::~CCrushEffect()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CCrushEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// 初期値
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CCrushEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCrushEffect::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// 体力の減少
	m_life--;

	D3DXCOLOR color = GetColor();
	color.a -= 0.025f;

	SetColor(color);

	// 死亡処理
	if (m_life <= 0 || color.a <= 0.0f)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CCrushEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 生成
//-----------------------------------------
CCrushEffect* CCrushEffect::Create(const D3DXVECTOR3& inPos)
{
	CCrushEffect* crush = new CCrushEffect;

	if (crush == nullptr)
	{
		return nullptr;
	}

	crush->Init();
	crush->SetPos(inPos);
	crush->SetSize(D3DXVECTOR2(10.0f, 10.0f));

	return bullet;
}
