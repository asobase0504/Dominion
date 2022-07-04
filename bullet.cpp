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
	m_life = 50;
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
// ����
//-----------------------------------------
CBullet* CBullet::Create(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inMove)
{
	CBullet* bullet = new CBullet;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->SetPos(inPos);
	bullet->m_move = inMove;
	bullet->SetSize(D3DXVECTOR2(10.0f, 10.0f));

	return bullet;
}
