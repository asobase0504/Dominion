//=========================================
// 
// プレイヤーからの入力命令クラス
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "player_controller.h"
#include "application.h"
#include "input.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CPlayerController::CPlayerController()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CPlayerController::~CPlayerController()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CPlayerController::Init()
{
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CPlayerController::Uninit()
{
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CPlayerController::Update()
{
}

//-----------------------------------------
// 移動
//-----------------------------------------
D3DXVECTOR3 CPlayerController::Move()
{
	CInput* input = CInput::GetKey();

	//CDirectInput* dinput = CApplication::GetInstance()->GetDirectInput();

	//bool isMove = false;
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (input == nullptr)
	{
		return moveInput;
	}

	// モデルの移動
	if (input->Press(DIK_W))
	{
		moveInput.y -= 1.0f;
		//isMove = true;
	}
	if (input->Press(DIK_A))
	{
		moveInput.x -= 1.0f;
		//isMove = true;
	}
	if (input->Press(DIK_S))
	{
		moveInput.y += 1.0f;
		//isMove = true;
	}
	if (input->Press(DIK_D))
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
CController::SHOT_TYPE CPlayerController::BulletShot()
{
	CInput* input = CInput::GetKey();

	if (input == nullptr)
	{
		return NONE_SHOT;
	}

	if (input->Press(DIK_UP) ||
		input->Press(DIK_DOWN) ||
		input->Press(DIK_LEFT) ||
		input->Press(DIK_RIGHT))
	{
		chargeCount++;
	}

	if (input->Release(DIK_UP) && chargeCount < 50)
	{
		chargeCount = 0;
		return UP_SHOT;
	}
	else if (input->Release(DIK_DOWN) && chargeCount < 50)
	{
		chargeCount = 0;
		return DOWN_SHOT;
	}
	else if (input->Release(DIK_LEFT) && chargeCount < 50)
	{
		chargeCount = 0;
		return LEFT_SHOT;
	}
	else if (input->Release(DIK_RIGHT) && chargeCount < 50)
	{
		chargeCount = 0;
		return RIGHT_SHOT;
	}
	else if (input->Release(DIK_UP) && chargeCount >= 50)
	{
		chargeCount = 0;
		return UP_CHARGE_SHOT;
	}
	else if (input->Release(DIK_DOWN) && chargeCount >= 50)
	{
		chargeCount = 0;
		return DOWN_CHARGE_SHOT;
	}
	else if (input->Release(DIK_LEFT) && chargeCount >= 50)
	{
		chargeCount = 0;
		return LEFT_CHARGE_SHOT;
	}
	else if (input->Release(DIK_RIGHT) && chargeCount >= 50)
	{
		chargeCount = 0;
		return RIGHT_CHARGE_SHOT;
	}

	if (chargeCount != 0)
	{
		return CHARGE_NOW;
	}

	return NONE_SHOT;
}
