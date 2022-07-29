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
#include "input_directinput.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "game.h"

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
	input(nullptr),
	directInput(nullptr),
	texture(nullptr),
	object(nullptr)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CApplication::~CApplication()
{
	assert(renderer == nullptr);
	assert(input == nullptr);
	assert(directInput == nullptr);
	assert(texture == nullptr);
	assert(object == nullptr);
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

	//directInput = new CDirectInput;
	//if (FAILED(directInput->Init(hInstance, hWnd)))
	//{
	//	return E_FAIL;
	//}

	// �����_�����O�N���X
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	// �e�N�X�`��
	texture = new CTexture;
	texture->LoadAll();

	// �Q�[�����[�h
	game = new CGame;
	if (FAILED(game->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CApplication::Uninit()
{
	// �I�u�W�F�N�g�S�̂̉��
	CObject::ReleaseAll();

	// �Q�[���̃N���A
	if (game != nullptr)
	{
		game = nullptr;
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

	// ���͏����̉��
	if (directInput != nullptr)
	{
		directInput->Uninit();

		delete directInput;
		directInput = nullptr;
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
	assert(application == nullptr);
}

//=============================================================================
// �X�V
//=============================================================================
void CApplication::Update()
{
	input->Update();
	//directInput->Update();
	renderer->Update();
}

//=============================================================================
// �`��
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}
