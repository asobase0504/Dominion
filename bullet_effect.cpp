//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "bullet_effect.h"
#include "application.h"
#include "stage.h"
#include "map.h"

//-----------------------------------------
// 定義
//-----------------------------------------

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CBulletEffect::CBulletEffect(CObject::TYPE type) :
	CObject2D(type,2),
	m_life(0)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CBulletEffect::~CBulletEffect()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CBulletEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// 初期値
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CBulletEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CBulletEffect::Update()
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
void CBulletEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 生成
//-----------------------------------------
CBulletEffect* CBulletEffect::Create(const D3DXVECTOR3& inPos,float inSize)
{
	CBulletEffect* bullet = new CBulletEffect;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->SetPos(inPos);

	float size = inSize * 0.25f;						// 大きさの設定
	bullet->SetSize(D3DXVECTOR2(size, size));

	return bullet;
}
