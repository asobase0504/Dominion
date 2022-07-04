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

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CCharacter::CCharacter()
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
	CApplication* application = CApplication::GetInstance();

	float moveLength = 0.0f;
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モデルの移動
	if (application->GetInput()->GetPress(DIK_W))
	{
		moveInput.y -= 1.0f;
		moveLength = 1.0f;
	}
	if (application->GetInput()->GetPress(DIK_A))
	{
		moveInput.x -= 1.0f;
		moveLength = 1.0f;
	}
	if (application->GetInput()->GetPress(DIK_S))
	{
		moveInput.y += 1.0f;
		moveLength = 1.0f;
	}
	if (application->GetInput()->GetPress(DIK_D))
	{
		moveInput.x += 1.0f;
		moveLength = 1.0f;
	}

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (moveLength > 0.0f)
	{
		D3DXVec3Normalize(&moveInput, &moveInput);

		move = moveInput;
	}

	// 方向ベクトル掛ける移動量
	m_pos += move * moveLength * 2.0f * 2.0f;
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
