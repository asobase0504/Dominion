//=========================================
// 
// �v���C���[����̓��͖��߃N���X
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
// �R���X�g���N�^
//-----------------------------------------
CPlayerOperate::CPlayerOperate()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CPlayerOperate::~CPlayerOperate()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CPlayerOperate::Init()
{
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CPlayerOperate::Uninit()
{
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CPlayerOperate::Update()
{
}

//-----------------------------------------
// �ړ�
//-----------------------------------------
D3DXVECTOR3 CPlayerOperate::Move()
{
	CApplication* application = CApplication::GetInstance();

	float moveLength = 0.0f;
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���f���̈ړ�
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
// �e�𔭎˂��鏈��
//-----------------------------------------
COperate::SHOT_TYPE CPlayerOperate::BulletShot()
{
	CApplication* application = CApplication::GetInstance();

	if (application->GetInput()->GetTrigger(DIK_UP))
	{
		return UP_SHOT;
	}
	else if (application->GetInput()->GetTrigger(DIK_DOWN))
	{
		return DOWN_SHOT;
	}
	else if (application->GetInput()->GetTrigger(DIK_LEFT))
	{
		return LEFT_SHOT;
	}
	else if (application->GetInput()->GetTrigger(DIK_RIGHT))
	{
		return RIGHT_SHOT;
	}

	return NONE_SHOT;
}
