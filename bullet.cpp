//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "bullet.h"
#include "application.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CBullet::CBullet() :
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_life(0)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CBullet::~CBullet()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CBullet::Init()
{
	CObject2D::Init();
	m_life = 200;	// �����l
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CBullet::Update()
{
	// �̗͂̌���
	m_life--;

	// �ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	ScreenFromOutTime();

	// ���S����
	if (m_life <= 0)
	{
		Release();
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// �`�[���̐ݒ�
//-----------------------------------------
void CBullet::SetTeam(const CCharacter::TEAM inTeam)
{
	m_team = inTeam;

	switch (m_team)
	{
	case CCharacter::TEAM_00:
		CObject2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �ʒu�̐ݒ�
		break;
	case CCharacter::TEAM_01:
		CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �ʒu�̐ݒ�
		break;
	default:
		break;
	}
}

//-----------------------------------------
// ����
//-----------------------------------------
CBullet* CBullet::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove, const CCharacter::TEAM inTeam)
{
	CBullet* bullet = new CBullet;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->SetPos(inPos);
	bullet->m_move = inMove;
	bullet->SetTeam(inTeam);
	bullet->SetSize(D3DXVECTOR2(10.0f, 10.0f));

	return bullet;
}

//-----------------------------------------
// �X�N���[���O�ɏo����
//-----------------------------------------
void CBullet::ScreenFromOutTime()
{
	if (m_pos.x + size.x <= 0.0f - size.x)
	{
		m_pos.x = static_cast<float>(CApplication::GetInstance()->SCREEN_WIDTH);
		SetPos(m_pos);
	}
	if (m_pos.x - size.x >= CApplication::GetInstance()->SCREEN_WIDTH + size.x)
	{
		m_pos.x = 0.0f;
		SetPos(m_pos);
	}
	if (m_pos.y + size.y <= 0.0f - size.y)
	{
		m_pos.y = static_cast<float>(CApplication::GetInstance()->SCREEN_HEIGHT);
		SetPos(m_pos);
	}
	if (m_pos.y - size.y >= CApplication::GetInstance()->SCREEN_HEIGHT + size.y)
	{
		m_pos.y = 0.0f;
		SetPos(m_pos);
	}
}
