//=========================================
// 
// �c�e�̕\���N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "remains_bullet.h"
#include "application.h"
#include "texture.h"
#include <assert.h>

#include "character.h"
//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CRemaubsBullet::CRemaubsBullet() : 
	CObject2D(),
	Parent(nullptr),
	rotY(0.0f)
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CRemaubsBullet::~CRemaubsBullet()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CRemaubsBullet::Init()
{
	CObject2D::Init();
	SetTexture("BULLET");
	SetSize(D3DXVECTOR2(6.0f, 6.0f));
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CRemaubsBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CRemaubsBullet::Update()
{
	rotY += 0.04f;
	if (rotY >= D3DX_PI * 2.0f)
	{
		rotY = 0.0f;
	}

	SetPos(D3DXVECTOR3(Parent->GetPos().x + sinf(rotY) * 8.0f, Parent->GetPos().y + cosf(rotY) * 8.0f,0.0f));
}

//-----------------------------------------
// �`��
//-----------------------------------------
void CRemaubsBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------
// �쐬
//-----------------------------------------
CRemaubsBullet* CRemaubsBullet::Create(CCharacter* inPerent, float inRotY)
{
	CRemaubsBullet* renaubsBullet = new CRemaubsBullet;

	if (renaubsBullet == nullptr)
	{
		return nullptr;
	}

	renaubsBullet->Init();
	renaubsBullet->Parent = inPerent;

	switch (renaubsBullet->Parent->GetTeam())
	{
	case TEAM_00:
		renaubsBullet->SetColor(CApplication::GetInstance()->GetColor(0));
		break;
	case TEAM_01:
		renaubsBullet->SetColor(CApplication::GetInstance()->GetColor(1));
		break;
	default:
		break;
	}
	renaubsBullet->rotY = inRotY;

	return renaubsBullet;
}
