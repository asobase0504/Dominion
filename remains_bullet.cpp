//=========================================
// 
// 残弾の表示クラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "remains_bullet.h"
#include "application.h"
#include "texture.h"
#include <assert.h>

#include "character.h"
//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CRemaubsBullet::CRemaubsBullet() : 
	CObject2D(),
	Parent(nullptr),
	rotY(0.0f)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CRemaubsBullet::~CRemaubsBullet()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CRemaubsBullet::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_BULLET);
	SetSize(D3DXVECTOR2(6.0f, 6.0f));
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CRemaubsBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CRemaubsBullet::Update()
{
	rotY += 0.04f;
	if (rotY >= D3DX_PI * 2.0f)
	{
		rotY = 0.0f;
	}

	SetPos(D3DXVECTOR3(Parent->GetPos().x + sinf(rotY) * 7.5f, Parent->GetPos().y + cosf(rotY) * 7.5f,0.0f));
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CRemaubsBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 作成
//-----------------------------------------
CRemaubsBullet* CRemaubsBullet::Create(CCharacter* inPerent, float inRotY)
{
	CRemaubsBullet* renaubsBullet = new CRemaubsBullet;

	if (renaubsBullet == nullptr)
	{
		return nullptr;
	}

	renaubsBullet->Init();
	renaubsBullet->Parent = inPerent;

	switch (renaubsBullet->Parent->GetTeam())
	{
	case TEAM_00:
		renaubsBullet->SetColor(CApplication::GetInstance()->GetColor(1));
		break;
	case TEAM_01:
		renaubsBullet->SetColor(CApplication::GetInstance()->GetColor(2));
		break;
	default:
		break;
	}
	renaubsBullet->rotY = inRotY;

	return renaubsBullet;
}
