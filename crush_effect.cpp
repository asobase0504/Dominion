//=========================================
// 
// �ӂ���G�t�F�N�g
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "crush_effect.h"
#include "application.h"

//-----------------------------------------
// ��`
//-----------------------------------------

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CCrushEffect::CCrushEffect(CObject::TYPE type) :
	CObject2D(type, 2),
	m_life(0)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CCrushEffect::~CCrushEffect()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CCrushEffect::Init()
{
	CObject2D::Init();
	m_life = 60;	// �����l
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CCrushEffect::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CCrushEffect::Update()
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
void CCrushEffect::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// ����
//-----------------------------------------
CCrushEffect* CCrushEffect::Create(const D3DXVECTOR3& inPos)
{
	CCrushEffect* crush = new CCrushEffect;

	if (crush == nullptr)
	{
		return nullptr;
	}

	crush->Init();
	crush->SetPos(inPos);
	crush->SetSize(D3DXVECTOR2(10.0f, 10.0f));

	return bullet;
}
