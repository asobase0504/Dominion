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
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	//bool isMove = false;
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モデルの移動
	if (input->GetPress(DIK_W))
	{
		moveInput.y -= 1.0f;
		//isMove = true;
	}
	if (input->GetPress(DIK_A))
	{
		moveInput.x -= 1.0f;
		//isMove = true;
	}
	if (input->GetPress(DIK_S))
	{
		moveInput.y += 1.0f;
		//isMove = true;
	}
	if (input->GetPress(DIK_D))
	{
		moveInput.x += 1.0f;
		//isMove = true;
	}

	if (/*isMove*/ moveInput.x != 0.0f || moveInput.y != 0.0f)
	{
		D3DXVec3Normalize(&moveInput, &moveInput);
	}

	return moveInput;
}

//-----------------------------------------
// 弾を発射する処理
//-----------------------------------------
COperate::SHOT_TYPE CPlayerOperate::BulletShot()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	if (input->GetRelease(DIK_UP))
	{
		return UP_SHOT;
	}
	else if (input->GetRelease(DIK_DOWN))
	{
		return DOWN_SHOT;
	}
	else if (input->GetRelease(DIK_LEFT))
	{
		return LEFT_SHOT;
	}
	else if (input->GetRelease(DIK_RIGHT))
	{
		return RIGHT_SHOT;
	}

	return NONE_SHOT;
}
