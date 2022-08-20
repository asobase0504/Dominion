//=============================================================================
// 
// �F�Ǘ��N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "theme_color.h"
#include "file.h"
#include "application.h"
#include "texture.h"

//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
static const nlohmann::json colorFile = LoadJsonStage(L"data/FILE/color.json");

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CThemeColor::CThemeColor()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CThemeColor::~CThemeColor()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CThemeColor::Init()
{
	SetTheme(0);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CThemeColor::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CThemeColor::Update()
{
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CThemeColor::Draw()
{
	// �����Ȃ�
}

//-----------------------------------------------------------------------------
// �F�̎擾
//-----------------------------------------------------------------------------
D3DXCOLOR CThemeColor::GetColor(int inConfig)
{
	// �w�肳�ꂽKey�����݂��邩�ۂ�
	if (m_color.count(inConfig) == 0)
	{ // Key���ݒ肳��ĂȂ������ꍇ
		return D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	}

	/* ��Key���ݒ肳��Ă����ꍇ�� */

	return m_color[inConfig];
}

//-----------------------------------------------------------------------------
// �e�[�}�F�̌���
//-----------------------------------------------------------------------------
void CThemeColor::SetTheme(int idx)
{
	// json�̃R���e�i��D3DXCOLOR�ɕϊ�����
	auto VectorToD3DXCOLOR = [this](int inIndex, int inColor)
	{
		// �w��ԍ��ɒl�����݂��Ă��邩�ۂ�
		if (colorFile["TYPE"].size() < inIndex || 0 > inIndex)
		{ // �w��ԍ��ɒl�����݂��Ȃ�
			return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		D3DXCOLOR color;

		color.r = colorFile["TYPE"].at(inIndex)["COLOR"].at(inColor)[0];	// �ԐF�̐ݒ�
		color.g = colorFile["TYPE"].at(inIndex)["COLOR"].at(inColor)[1];	// �ΐF�̐ݒ�
		color.b = colorFile["TYPE"].at(inIndex)["COLOR"].at(inColor)[2];	// �F�̐ݒ�
		color.a = colorFile["TYPE"].at(inIndex)["COLOR"].at(inColor)[3];	// �����F�̐ݒ�

		return color;
	};

	// �F�̑��
	m_color.insert(std::make_pair(0, VectorToD3DXCOLOR(idx, 0)));	// ���C���F1
	m_color.insert(std::make_pair(1, VectorToD3DXCOLOR(idx, 1)));	// ���C���F2
	m_color.insert(std::make_pair(2, VectorToD3DXCOLOR(idx, 2)));	// �w�i�F
	m_color.insert(std::make_pair(3, VectorToD3DXCOLOR(idx, 3)));	// �T�u�F

	if(colorFile["TYPE"].at(idx).count("BG") != 0)
	{
		CApplication::GetInstance()->GetTextureClass()->Load("BG",colorFile["TYPE"].at(idx)["BG"]);
	}
}
