//=========================================
// 
// �v���C���[�N���X
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
// �R���X�g���N�^
//-----------------------------------------
CCharacter::CCharacter()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CCharacter::~CCharacter()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CCharacter::Init()
{
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE::TEXTURE_PLAYER);
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CCharacter::Uninit()
{
	CObject2D::Uninit();

}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CCharacter::Update()
{
	Move();
	BulletShot();
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CCharacter::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// �ړ�
//-----------------------------------------
void CCharacter::Move()
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

	// �����x�N�g���|����ړ���
	m_pos += move * moveLength * 2.0f * 2.0f;
	CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
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
// ����
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
