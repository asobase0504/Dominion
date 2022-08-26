//=========================================
// 
// �v���C���[����̓��͖��߃N���X
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "player_controller.h"
#include "application.h"
#include "input_keybord.h"
#include "input_directinput.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CPlayerController::CPlayerController()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CPlayerController::~CPlayerController()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CPlayerController::Init()
{
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CPlayerController::Uninit()
{
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CPlayerController::Update()
{
}

//-----------------------------------------
// �ړ�
//-----------------------------------------
D3DXVECTOR3 CPlayerController::Move()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	//CDirectInput* dinput = CApplication::GetInstance()->GetDirectInput();

	//bool isMove = false;
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (input == nullptr)
	{
		return moveInput;
	}

	// ���f���̈ړ�
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
// �e�𔭎˂��鏈��
//-----------------------------------------
CController::SHOT_TYPE CPlayerController::BulletShot()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	if (input == nullptr)
	{
		return NONE_SHOT;
	}

	if (input->GetPress(DIK_UP) ||
		input->GetPress(DIK_DOWN) ||
		input->GetPress(DIK_LEFT) ||
		input->GetPress(DIK_RIGHT))
	{
		chargeCount++;
	}

	if (input->GetRelease(DIK_UP) && chargeCount < 50)
	{
		chargeCount = 0;
		return UP_SHOT;
	}
	else if (input->GetRelease(DIK_DOWN) && chargeCount < 50)
	{
		chargeCount = 0;
		return DOWN_SHOT;
	}
	else if (input->GetRelease(DIK_LEFT) && chargeCount < 50)
	{
		chargeCount = 0;
		return LEFT_SHOT;
	}
	else if (input->GetRelease(DIK_RIGHT) && chargeCount < 50)
	{
		chargeCount = 0;
		return RIGHT_SHOT;
	}
	else if (input->GetRelease(DIK_UP) && chargeCount >= 50)
	{
		chargeCount = 0;
		return UP_CHARGE_SHOT;
	}
	else if (input->GetRelease(DIK_DOWN) && chargeCount >= 50)
	{
		chargeCount = 0;
		return DOWN_CHARGE_SHOT;
	}
	else if (input->GetRelease(DIK_LEFT) && chargeCount >= 50)
	{
		chargeCount = 0;
		return LEFT_CHARGE_SHOT;
	}
	else if (input->GetRelease(DIK_RIGHT) && chargeCount >= 50)
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
