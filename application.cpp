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
#include "color.h"
#include "texture.h"
#include "object2d.h"
#include "title.h"
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
	SCREEN_WIDTH(1280),
	SCREEN_HEIGHT(720),
	CENTER_X(SCREEN_WIDTH * 0.5f),
	CENTER_Y(SCREEN_HEIGHT * 0.5f),
	mode(nullptr),
	renderer(nullptr),
	input(nullptr),
	directInput(nullptr),
	texture(nullptr),
	object(nullptr),
	color(nullptr)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CApplication::~CApplication()
{
	assert(mode == nullptr);
	assert(renderer == nullptr);
	assert(input == nullptr);
	assert(directInput == nullptr);
	assert(texture == nullptr);
	assert(object == nullptr);
	assert(color == nullptr);
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

	// �F�Ǘ��N���X
	color = new CColor;
	if (FAILED(color->Init()))
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
	texture->LoadAll();

	// �Q�[�����[�h
	SetMode(MODE_TYPE::TITLE);

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
	if (mode != nullptr)
	{
		mode->Uninit();

		delete mode;
		mode = nullptr;
	}

	// �e�N�X�`���̉��
	texture->UnloadAll();
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}

	// �F�Ǘ��̉��
	if (color != nullptr)
	{
		delete color;
		color = nullptr;
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
	mode->Update();
}

//=============================================================================
// �`��
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}

D3DXCOLOR CApplication::GetColor(std::string inKey)
{
	return color->GetColor(inKey);
}

void CApplication::SetMode(MODE_TYPE inType)
{
	if (mode != nullptr)
	{
		CObject::ReleaseAll();
		mode->Uninit();
		delete mode;
		mode = nullptr;
	}

	switch (inType)
	{
	case CApplication::MODE_TYPE::TITLE:
		mode = new CTitle;
		break;
	case CApplication::MODE_TYPE::GAME:
		mode = new CGame;
		break;
	case CApplication::MODE_TYPE::RESULT:
		break;
	default:
		break;
	}

	if (FAILED(mode->Init()))
	{
		assert(false);
	}
}
