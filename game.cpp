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
	CApplication::GetInstance()->SetThemeColor(0);

	stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	// �w�i�̐ݒ�
	{
		CObject2D* bg = CObject2D::Create(CObject::TYPE::NONE,1);
		bg->SetSize(D3DXVECTOR2((float)CApplication::GetInstance()->SCREEN_WIDTH, (float)CApplication::GetInstance()->SCREEN_HEIGHT));
		D3DXVECTOR3 pos(CApplication::GetInstance()->CENTER_X, CApplication::GetInstance()->CENTER_Y, 0.0f);	// �ʒu�̎擾
		bg->SetTexture("BG");
		bg->SetPos(pos);
		bg->SetColor(CApplication::GetInstance()->GetColor(2));
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
		controller.push_back(inController);
		int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// X�̈ʒu�ԍ����擾
		int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Y�̈ʒu�ԍ����擾
		CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// �`�[���̍쐬
		character.push_back(CCharacter::Create(inTeam));				// ����
		float size = map->GetBlockSize() * 0.65f;
		character.at(inIdx)->SetSize(D3DXVECTOR2(size, size));			// �傫���̐ݒ�
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// �ʒu�̎擾
		character.at(inIdx)->SetPos(pos);								// �ʒu�̐ݒ�
		character.at(inIdx)->SetCenterBlockIndex({ x,y });				// �����ʒu�̐ݒ�
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

	for (auto it = controller.begin(); it != controller.end();)
	{
		// �R���g���[���[�̍폜
		(*it)->Uninit();
		delete (*it);
		(*it) = nullptr;

		it = controller.erase(it);
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

	for (int i = 0; i < character.size(); i++)
	{
		if (character[i]->GetIsDeleted())
		{
			CApplication::GetInstance()->SetMode(CApplication::MODE_TYPE::TITLE);
		}
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}
