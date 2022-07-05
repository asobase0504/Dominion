//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "character.h"
#include "application.h"
#include "input_keybord.h"
#include "bullet.h"
#include "operate.h"
#include <assert.h>

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCharacter::CCharacter() : 
	m_operate(nullptr)
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CCharacter::~CCharacter()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CCharacter::Uninit()
{
	CObject2D::Uninit();

}

//-----------------------------------------
// 更新
//-----------------------------------------
void CCharacter::Update()
{
	Move();
	BulletShot();
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CCharacter::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// 移動
//-----------------------------------------
void CCharacter::Move()
{
	if (m_operate == nullptr)
	{
		assert(false);
		return;
	}

	// 方向ベクトル掛ける移動量
	m_pos += m_operate->Move() * 2.0f * 2.0f;
	CObject2D::SetPos(m_pos);		// 位置の設定
}

void CCharacter::BulletShot()
{
	CApplication* application = CApplication::GetInstance();

	if (application->GetInput()->GetTrigger(DIK_UP))
	{
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	}
	else if (application->GetInput()->GetTrigger(DIK_DOWN))
	{
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	}
	else if (application->GetInput()->GetTrigger(DIK_LEFT))
	{
		CBullet::Create(m_pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
	}
	else if (application->GetInput()->GetTrigger(DIK_RIGHT))
	{
		CBullet::Create(m_pos, D3DXVECTOR3(10.0f, 0.0f, 0.0f));
	}
}

//-----------------------------------------
// 生成
//-----------------------------------------
CCharacter* CCharacter::Create()
{
	CCharacter* player = new CCharacter;

	if (player == nullptr)
	{
		return nullptr;
	}

	player->Init();

	return player;
}
