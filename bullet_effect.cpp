//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "bullet_effect.h"
#include "application.h"
#include "stage.h"
#include "map.h"

//-----------------------------------------
// ��`
//-----------------------------------------

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CBulletEffect::CBulletEffect(CObject::TYPE type) :
	CObject2D(type,2),
	m_life(0)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CBulletEffect::~CBulletEffect()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CBulletEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// �����l
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CBulletEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CBulletEffect::Update()
{
	if (GetIsDeleted())
	{
		return;
	}

	// �̗͂̌���
	m_life--;

	D3DXCOLOR color = GetColor();
	color.a -= 0.025f;

	SetColor(color);

	// ���S����
	if (m_life <= 0 || color.a <= 0.0f)
	{
		m_isDeleted = true;
	}
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CBulletEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// ����
//-----------------------------------------
CBulletEffect* CBulletEffect::Create(const D3DXVECTOR3& inPos,float inSize)
{
	CBulletEffect* bullet = new CBulletEffect;

	if (bullet == nullptr)
	{
		return nullptr;
	}

	bullet->Init();
	bullet->SetPos(inPos);

	float size = inSize * 0.25f;						// �傫���̐ݒ�
	bullet->SetSize(D3DXVECTOR2(size, size));

	return bullet;
}
