//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"
#include <assert.h>
#include <functional>

#include "application.h"
#include "input.h"
#include "input_keybord.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGame::CGame() :
	//character(nullptr),
	map(nullptr)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
	assert(character.empty());
	assert(map == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGame::Init()
{
	stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create();
		bg->SetSize(D3DXVECTOR2(CApplication::GetInstance()->SCREEN_WIDTH, CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->SCREEN_WIDTH * 0.5f, CApplication::GetInstance()->SCREEN_HEIGHT * 0.5f, 0.0f);	// �ʒu�̎擾
		bg->SetTexture(CTexture::TEXTURE::TEXTERE_BG);
		bg->SetPos(pos);
		bg->SetColor(D3DXCOLOR(0.45f, 0.45f, 0.9f, 1.0f));
	}

	// �}�b�v�N���X
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	// �v���C���[�������_���ŃN���G�C�g
	std::function<void(int, CController*)> playerSet;

	playerSet = [this](int inIdx, CController* inController)->void
	{
		int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// X�̈ʒu�ԍ����擾
		int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Y�̈ʒu�ԍ����擾
		CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// �`�[���̍쐬
		character.push_back(CCharacter::Create(inTeam));				// ����
		character.at(inIdx)->SetSize(D3DXVECTOR2(25.0f, 25.0f));		// �傫���̐ݒ�
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// �ʒu�̎擾
		character.at(inIdx)->SetPos(pos);								// �ʒu�̐ݒ�
		character.at(inIdx)->SetBlockIndex(0, { x,y });					// �����u���b�N��ݒ�
		character.at(inIdx)->SetController(inController);				// ���ߎ҂̐ݒ�
	};

	// �L�����N�^�[�̐ݒ�
	playerSet(0, new CAIController);
	playerSet(1, new CPlayerController);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	for (auto it = character.begin(); it != character.end();)
	{
		it = character.erase(it);
	}

	// �}�b�v�̉��
	if (map != nullptr)
	{
		map->Uninit();

		delete map;
		map = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update()
{
	CInputKeybord* input = CApplication::GetInstance()->GetInput();

	//	��ʂ̑J��
	if (input->GetTrigger(DIK_RETURN))
	{
		CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}
