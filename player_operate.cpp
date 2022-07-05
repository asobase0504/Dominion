//=========================================
// 
// プレイヤーからの入力命令クラス
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "player_operate.h"
#include "application.h"
#include "input_keybord.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CPlayerOperate::CPlayerOperate()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CPlayerOperate::~CPlayerOperate()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CPlayerOperate::Init()
{
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CPlayerOperate::Uninit()
{
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CPlayerOperate::Update()
{
}

//-----------------------------------------
// 移動
//-----------------------------------------
D3DXVECTOR3 CPlayerOperate::Move()
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

	return move;
}

//-----------------------------------------
// 弾を発射する処理
//-----------------------------------------
bool CPlayerOperate::BulletShot()
{
	return false;
}
