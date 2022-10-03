//=============================================================================
// 
// �`���[�g���A���N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "tutorial.h"
#include "stage.h"
#include "input.h"
#include <assert.h>
#include <tchar.h> // _T

#include "renderer.h"
#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CTutorial::CTutorial() :
	m_stageIndex(0),
	m_controllerIndex(0),
	m_stage(nullptr),
	m_content(NONE),
	isResetText(true),
	chargeCount(0)
{
	m_isSuccess.clear();
	m_stageInfo.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CTutorial::~CTutorial()
{
	assert(m_stage == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CTutorial::Init()
{
	// �t�H���g�̃��[�h
	AddFontResourceEx("data/FONT/Harenosora.otf", FR_PRIVATE, &m_design);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// ��ڂ̃t�H���g�̐���
	D3DXCreateFont(device, 55, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Harenosora"), &m_fontSubject);

	// ���ڂ̃t�H���g�̐���
	D3DXCreateFont(device, 28, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Harenosora"), &m_font);

	{ // �w�i�̐ݒ�
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE, 0);
		bg->SetSize(CApplication::GetInstance()->GetScreenSize());
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
	}

	// �X�e�[�W���̎擾
	m_stageInfo = LoadJsonStage(L"data/FILE/STAGE/stage02.json").at("STAGE");

	ResetStage();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CTutorial::Uninit()
{
	RemoveFontResourceEx(TEXT("data/FONT/Harenosora.otf"), FR_PRIVATE, &m_design);

	if (m_fontSubject != nullptr)
	{
		m_fontSubject->Release();
		m_fontSubject = nullptr;
	}

	if (m_font != nullptr)
	{
		m_font->Release();
		m_font = nullptr;
	}

	if (m_stage != nullptr)
	{
		m_stage->Uninit();
		delete m_stage;
		m_stage = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CTutorial::Update()
{
	m_stage->Update();

	// �S�Ă̍��ڂ��N���A���Ă邩�ۂ�
	for (int i = 0; i < (int)m_isSuccess.size(); i++)
	{
		if (!m_isSuccess.at(i))
		{
			isResetText = false;
			break;
		}
		isResetText = true;
	}

	if (isResetText)
	{
		switch (m_content)
		{
		case CTutorial::NONE:
			m_content = MOVE;
			break;
		case CTutorial::MOVE:
			m_content = SHOT;
			break;
		case CTutorial::SHOT:
			m_content = CHARGE;
			break;
		case CTutorial::CHARGE:
			CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
			return;
			break;
		default:
			break;
		}
	}
	
	CheckMission();
	ResetText();

	bool isKeybord = CInput::GetKey()->Trigger(DIK_BACKSPACE) && m_controllerIndex == -1;
	bool isJoyPad = CInput::GetKey()->Trigger(JOYPAD_BACK, m_controllerIndex) && m_controllerIndex != -1;
	if (isKeybord || isJoyPad)
	{
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CTutorial::Draw()
{
	TCHAR str[256];

	{
		RECT rect = { 0, 50, CApplication::GetInstance()->SCREEN_WIDTH, CApplication::GetInstance()->SCREEN_HEIGHT };
		wsprintf(str, _T(m_textSubject.c_str()));

		m_fontSubject->DrawText(NULL, str, -1, &rect, DT_CENTER, CApplication::GetInstance()->GetColor(0));
	}

	for (int i = 0; i < m_text.size(); i++)
	{
		RECT rect = { 0, 550 + 28 * i, CApplication::GetInstance()->SCREEN_WIDTH, CApplication::GetInstance()->SCREEN_HEIGHT };
		wsprintf(str, _T(m_text.at(i).c_str()));

		m_font->DrawText(NULL, str, -1, &rect, DT_CENTER, m_isSuccess.at(i) ? CApplication::GetInstance()->GetColor(3) :CApplication::GetInstance()->GetColor(0));
	}
}

//-----------------------------------------------------------------------------
// �R���g���[���[�̏���ݒ�
//-----------------------------------------------------------------------------
void CTutorial::SetControllerIndex(const int inControllerIndex)
{
	m_controllerIndex = inControllerIndex;
	m_stage->SetControllerIndex({ inControllerIndex });
}

//-----------------------------------------------------------------------------
// �X�e�[�W�̃��Z�b�g
//-----------------------------------------------------------------------------
void CTutorial::ResetStage()
{
	if (m_stage != nullptr)
	{
		m_stage->AllDelete();	// �X�e�[�W��ō�����I�u�W�F�N�g��S�ĉ��
		m_stage->Uninit();	// �I��
		delete m_stage;
		m_stage = nullptr;
	}

	m_stage = new CStage;
	m_stage->Init(m_stageInfo.at(m_stageIndex));	// ������
}

//-----------------------------------------------------------------------------
// �e�L�X�g�̃��Z�b�g
//-----------------------------------------------------------------------------
void CTutorial::ResetText()
{
	if (!isResetText)
	{
		return;
	}

	/* ���e�L�X�g�����Z�b�g����ꍇ�� */

	isResetText = false;

	if (m_controllerIndex == -1)
	{
		switch (m_content)
		{
		case CTutorial::MOVE:
		{
			m_textSubject = "�ړ�";
			m_text.resize(4);
			m_isSuccess.resize(4);
			m_isSuccess.at(0) = false;
			m_isSuccess.at(1) = false;
			m_isSuccess.at(2) = false;
			m_isSuccess.at(3) = false;
			m_text.at(0) = "�E�v�L�[�������ď�Ɉړ�\n";
			m_text.at(1) = "�E�`�L�[�������č��Ɉړ�\n";
			m_text.at(2) = "�E�r�L�[�������ĉ��Ɉړ�\n";
			m_text.at(3) = "�E�c�L�[�������ĉE�Ɉړ�\n";
		}
		break;
		case CTutorial::SHOT:
		{
			m_textSubject = "�V���b�g";
			m_text.resize(4);
			m_isSuccess.resize(4);
			m_isSuccess.at(0) = false;
			m_isSuccess.at(1) = false;
			m_isSuccess.at(2) = false;
			m_isSuccess.at(3) = false;
			m_text.at(0) = "�E���L�[�������ď�Ɍ���\n";
			m_text.at(1) = "�E���L�[�������č��Ɍ���\n";
			m_text.at(2) = "�E���L�[�������ĉ��Ɍ���\n";
			m_text.at(3) = "�E���L�[�������ĉE�Ɍ���\n";
		}
		break;
		case CTutorial::CHARGE:
		{
			m_textSubject = "�`���[�W�V���b�g";
			m_text.resize(4);
			m_isSuccess.resize(4);
			m_isSuccess.at(0) = false;
			m_isSuccess.at(1) = false;
			m_isSuccess.at(2) = false;
			m_isSuccess.at(3) = false;
			m_text.at(0) = "�E���L�[�𒷉����ď�Ɍ���\n";
			m_text.at(1) = "�E���L�[�𒷉����č��Ɍ���\n";
			m_text.at(2) = "�E���L�[�𒷉����ĉ��Ɍ���\n";
			m_text.at(3) = "�E���L�[�𒷉����ĉE�Ɍ���\n";
		}
		break;
		default:
			break;
		}
	}
	else
	{
		switch (m_content)
		{
		case CTutorial::MOVE:
		{
			m_textSubject = "�ړ�";
			m_text.resize(1);
			m_isSuccess.resize(1);
			m_isSuccess.at(0) = false;
			m_text.at(0) = "�E���X�e�B�b�N�𓮂����Ĉړ�\n";
		}
		break;
		case CTutorial::SHOT:
		{
			m_textSubject = "�V���b�g";
			m_text.resize(4);
			m_isSuccess.resize(4);
			m_isSuccess.at(0) = false;
			m_isSuccess.at(1) = false;
			m_isSuccess.at(2) = false;
			m_isSuccess.at(3) = false;
			m_text.at(0) = "�E�x�L�[�������ď�Ɍ���\n";
			m_text.at(1) = "�E�w�L�[�������č��Ɍ���\n";
			m_text.at(2) = "�E�`�L�[�������ĉ��Ɍ���\n";
			m_text.at(3) = "�E�a�L�[�������ĉE�Ɍ���\n";
		}
		break;
		case CTutorial::CHARGE:
		{
			m_textSubject = "�`���[�W�V���b�g";
			m_text.resize(4);
			m_isSuccess.resize(4);
			m_isSuccess.at(0) = false;
			m_isSuccess.at(1) = false;
			m_isSuccess.at(2) = false;
			m_isSuccess.at(3) = false;
			m_text.at(0) = "�E�x�L�[�𒷉����ď�Ɍ���\n";
			m_text.at(1) = "�E�w�L�[�𒷉����č��Ɍ���\n";
			m_text.at(2) = "�E�`�L�[�𒷉����ĉ��Ɍ���\n";
			m_text.at(3) = "�E�a�L�[�𒷉����ĉE�Ɍ���\n";
		}
		break;
		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// �~�b�V�����̏󋵊m�F
//-----------------------------------------------------------------------------
void CTutorial::CheckMission()
{
	if (m_isSuccess.empty())
	{
		return;
	}
	CInput* input = CInput::GetKey();
	if (m_controllerIndex == -1)
	{
		switch (m_content)
		{
		case CTutorial::MOVE:
		{
			if (input->Press(DIK_W))
			{
				m_isSuccess.at(0) = true;
			}
			if (input->Press(DIK_A))
			{
				m_isSuccess.at(1) = true;
			}
			if (input->Press(DIK_S))
			{
				m_isSuccess.at(2) = true;
			}
			if (input->Press(DIK_D))
			{
				m_isSuccess.at(3) = true;
			}
		}
		break;
		case CTutorial::SHOT:
		{
			if (input->Release(DIK_UP))
			{
				m_isSuccess.at(0) = true;
			}
			if (input->Release(DIK_LEFT))
			{
				m_isSuccess.at(1) = true;
			}
			if (input->Release(DIK_DOWN))
			{
				m_isSuccess.at(2) = true;
			}
			if (input->Release(DIK_RIGHT))
			{
				m_isSuccess.at(3) = true;
			}
		}
		break;
		case CTutorial::CHARGE:
		{
			if (input->Press(DIK_UP) ||
				input->Press(DIK_LEFT) ||
				input->Press(DIK_DOWN) ||
				input->Press(DIK_RIGHT))
			{
				chargeCount++;
			}

			if (input->Release(DIK_UP))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(0) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(DIK_LEFT))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(1) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(DIK_DOWN))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(2) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(DIK_RIGHT))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(3) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
		}
		break;
		default:
			break;
		}
	}
	else
	{
		switch (m_content)
		{
		case CTutorial::MOVE:
		{
			if (input->Press(KEY_MOVE_UP, m_controllerIndex) ||
				input->Press(KEY_MOVE_DOWN, m_controllerIndex) ||
				input->Press(KEY_MOVE_LEFT, m_controllerIndex) ||
				input->Press(KEY_MOVE_RIGHT, m_controllerIndex))
			{
				m_isSuccess.at(0) = true;
			}
		}
		break;
		case CTutorial::SHOT:
		{
			if (input->Release(KEY_SHOT_UP, m_controllerIndex))
			{
				m_isSuccess.at(0) = true;
			}
			if (input->Release(KEY_SHOT_LEFT, m_controllerIndex))
			{
				m_isSuccess.at(1) = true;
			}
			if (input->Release(KEY_SHOT_DOWN, m_controllerIndex))
			{
				m_isSuccess.at(2) = true;
			}
			if (input->Release(KEY_SHOT_RIGHT, m_controllerIndex))
			{
				m_isSuccess.at(3) = true;
			}
		}
		break;
		case CTutorial::CHARGE:
		{
			if (input->Press(KEY_SHOT_UP, m_controllerIndex) ||
				input->Press(KEY_SHOT_LEFT, m_controllerIndex) ||
				input->Press(KEY_SHOT_DOWN, m_controllerIndex) ||
				input->Press(KEY_SHOT_RIGHT, m_controllerIndex))
			{
				chargeCount++;
			}

			if (input->Release(KEY_SHOT_UP, m_controllerIndex))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(0) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(KEY_SHOT_LEFT, m_controllerIndex))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(1) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(KEY_SHOT_DOWN, m_controllerIndex))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(2) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
			if (input->Release(KEY_SHOT_RIGHT, m_controllerIndex))
			{
				if (chargeCount >= 50)
				{
					m_isSuccess.at(3) = true;
					chargeCount = 0;
				}
				else
				{
					chargeCount = 0;
				}
			}
		}
		break;
		default:
			break;
		}
	}
}
