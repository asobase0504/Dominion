//=========================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=========================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include <assert.h>
#include "input_keybord.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�[�ϐ��̏�����
//-----------------------------------------------------------------------------
CApplication* CApplication::application = nullptr;

//=============================================================================
// �V���O���g���ł̃C���X�^���X�̎擾
//=============================================================================
CApplication* CApplication::GetInstance()
{
	if (application == nullptr)
	{
		application = new CApplication;
	}

	return application;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CApplication::CApplication() :
	renderer(nullptr),
	object(nullptr)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CApplication::~CApplication()
{
	assert(object == nullptr);
	assert(renderer == nullptr);
	assert(character == nullptr);
	assert(input == nullptr);
	assert(texture == nullptr);
}

//=============================================================================
// ������
//=============================================================================
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
	// ���͏���
	input = new CInputKeybord;
	if (FAILED(input->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �����_�����O�N���X
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	// �e�N�X�`��
	texture = new CTexture;
	//texture->LoadAll();

	// �}�b�v�N���X
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->Set();

	// �v���C���[�N���X
	character = CCharacter::Create();
	character->SetPos(D3DXVECTOR3(800.0f, 300.0f, 0.0f));	// �ʒu�̐ݒ�
	character->SetSize(D3DXVECTOR2(20.0f, 20.0f));	// �傫���̐ݒ�
	character->SetController(new CPlayerController);	// ���ߎ҂̐ݒ�

	character = CCharacter::Create();
	character->SetPos(D3DXVECTOR3(200.0f, 300.0f, 0.0f));	// �ʒu�̐ݒ�
	character->SetSize(D3DXVECTOR2(20.0f, 20.0f));	// �傫���̐ݒ�
	character->SetController(new CPlayerController);	// ���ߎ҂̐ݒ�

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CApplication::Uninit()
{
	// �I�u�W�F�N�g�S�̂̉��
	CObject::ReleaseAll();

	// �v���C���[�̃N���A
	if (character != nullptr)
	{
		character = nullptr;
	}

	// �e�N�X�`���̉��
	texture->UnloadAll();
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}

	// ���͏����̉��
	if (input != nullptr)
	{
		input->Uninit();

		delete input;
		input = nullptr;
	}

	// �����_���[�̉��
	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	// �A�v���P�[�V�����̉��
	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CApplication::Update()
{
	input->Update();
	renderer->Update();
}

//=============================================================================
// �`��
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}

//=============================================================================
// �����_�����O�̃C���X�^���X�擾
//=============================================================================
CRenderer* CApplication::GetRenderer()
{
	return renderer;
}

//=============================================================================
// �I�u�W�F�N�g�̃C���X�^���X�擾
//=============================================================================
CObject** CApplication::GetMyObject()
{
	return &object;
}

//=============================================================================
// ���͏����̎擾
//=============================================================================
CInputKeybord* CApplication::GetInput()
{
	return input;
}

//=============================================================================
// �e�N�X�`���N���X�̎擾
//=============================================================================
CTexture* CApplication::GetTextureClass()
{
	return texture;
}
