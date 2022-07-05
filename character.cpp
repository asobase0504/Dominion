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
#include "operate.h"
#include <assert.h>

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CCharacter::CCharacter() : 
	m_operate(nullptr)
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

	if (m_pos.x + size.x <= 0.0f - size.x)
	{
		m_pos.x = CApplication::GetInstance()->SCREEN_WIDTH;
		SetPos(m_pos);
	}
	if (m_pos.x - size.x >= CApplication::GetInstance()->SCREEN_WIDTH + size.x)
	{
		m_pos.x = 0.0f;
		SetPos(m_pos);
	}
	if (m_pos.y + size.y <= 0.0f - size.y)
	{
		m_pos.y = CApplication::GetInstance()->SCREEN_HEIGHT;
		SetPos(m_pos);
	}
	if (m_pos.y - size.y >= CApplication::GetInstance()->SCREEN_HEIGHT + size.y)
	{
		m_pos.y = 0.0f;
		SetPos(m_pos);
	}
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
	if (m_operate == nullptr)
	{
		assert(false);
		return;
	}

	// �����x�N�g���|����ړ���
	m_pos += m_operate->Move() * 2.0f * 2.0f;
	CObject2D::SetPos(m_pos);		// �ʒu�̐ݒ�
}

void CCharacter::BulletShot()
{
	if (m_operate == nullptr)
	{
		assert(false);
		return;
	}

	switch (m_operate->BulletShot())
	{
	case COperate::UP_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -10.0f, 0.0f));
		break;
	case COperate::DOWN_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		break;
	case COperate::LEFT_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
		break;
	case COperate::RIGHT_SHOT:
		CBullet::Create(m_pos, D3DXVECTOR3(10.0f, 0.0f, 0.0f));
		break;

	default:
		break;
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
