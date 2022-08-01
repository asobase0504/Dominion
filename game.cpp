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

#include "file.h"

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
	nlohmann::json stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	// �}�b�v�N���X
	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	// �L�����N�^�[�̐ݒ�
	{
		int x = stage["PLAYERS"].at(0)["SPAWN"].at(0);
		int y = stage["PLAYERS"].at(0)["SPAWN"].at(1);
		character.push_back(CCharacter::Create(stage["PLAYERS"].at(0)["TYPE"], x));	// ����
		character.at(0)->SetSize(D3DXVECTOR2(20.0f, 20.0f));			// �傫���̐ݒ�
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();
		character.at(0)->SetPos(pos);
		character.at(0)->SetController(new CPlayerController);				// ���ߎ҂̐ݒ�
	}

	// �L�����N�^�[�̐ݒ�
	{
		int x = stage["PLAYERS"].at(1)["SPAWN"].at(0);
		int y = stage["PLAYERS"].at(1)["SPAWN"].at(1);
		character.push_back(CCharacter::Create(stage["PLAYERS"].at(1)["TYPE"],x));	// ����
		character.at(1)->SetSize(D3DXVECTOR2(20.0f, 20.0f));						// �傫���̐ݒ�
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();
		character.at(1)->SetPos(pos);
		character.at(1)->SetController(new CPlayerController);				// ���ߎ҂̐ݒ�
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGame::Uninit()
{
	for (auto it = character.begin(); it != character.end();)
	{
		(*it)->Uninit();
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update()
{
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw()
{
}