#include "stage.h"
#include "application.h"
#include "character.h"
#include "block.h"
#include "map.h"
#include "player_controller.h"
#include "AI_controller.h"
#include "ui_countdown.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CStage::CStage() :
	map(nullptr),
	m_isEndGame(false),
	m_isPreparing(false),
	m_PreparingCount(0)
{
	character.clear();
	controller.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CStage::~CStage()
{
	assert(character.empty());
	assert(controller.empty());
	assert(map == nullptr);
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CStage::Init()
{
	// �}�b�v�N���X
	stage = LoadJsonStage(L"data/FILE/STAGE/stage01.json");

	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	m_isPreparing = true;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CStage::Uninit()
{
	DeleteBullet();

	// �L�����N�^�[�̉��
	for (auto it = character.begin(); it != character.end();)
	{
		it = character.erase(it);
	}

	// �R���g���[���[�̉��
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
void CStage::Update()
{
	if (m_isPreparing)
	{
		if (m_PreparingCount <= CCountDownUI::READY_TIME + CCountDownUI::GO_TIME)
		{
			m_PreparingCount++;
			return;
		}

		// �v���C���[�������_���ŃN���G�C�g
		std::function<void(int, CController*)> playerSet;

		playerSet = [this](int inIdx, CController* inController)->void
		{
			controller.push_back(inController);
			int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// X�̈ʒu�ԍ����擾
			int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Y�̈ʒu�ԍ����擾
			CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// �`�[���̍쐬
			character.push_back(CCharacter::Create(inTeam));				// ����
			float size = map->GetBlockSize() * 0.65f;						// �傫���̐ݒ�
			character.at(inIdx)->SetSize(D3DXVECTOR2(size, size));			// �傫���̑��
			D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// �ʒu�̎擾
			character.at(inIdx)->SetPos(pos);								// �ʒu�̐ݒ�
			character.at(inIdx)->SetCenterBlockIndex({ x,y });				// �����ʒu�̐ݒ�
			character.at(inIdx)->SetBlockIndex(0, { x,y });					// �����u���b�N��ݒ�
			character.at(inIdx)->SetController(inController);				// ���ߎ҂̐ݒ�
		};

		// �L�����N�^�[�̐ݒ�
		playerSet(0, new CAIController);
		playerSet(1, new CPlayerController);
		m_isPreparing = false;
	}

	for (int i = 0; i < character.size(); i++)
	{
		if (character[i]->GetIsDeleted())
		{
			// ���S��;
			m_isEndGame = true;
		}
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CStage::Draw()
{
}

//-----------------------------------------------------------------------------
// �e�̍폜�˗�
//-----------------------------------------------------------------------------
void CStage::DeleteBullet()
{
	// �I�u�W�F�N�g�S�̂��擾
	for (auto it = CObject::GetMyObject(2)->begin(); it != CObject::GetMyObject(2)->end(); it++)
	{
		if ((*it)->CObject::GetType() == CObject::TYPE::BULLET)
		{
			(*it)->SetIsDeleted(true);
		}
	}
}

//-----------------------------------------------------------------------------
// �X�e�[�W���S�Ă̍폜�˗�
//-----------------------------------------------------------------------------
void CStage::AllDelete()
{
	for (auto it = character.begin(); it != character.end();)
	{
		(*it)->SetIsDeleted(true);
		it = character.erase(it);
	}
	map->Delete();
}
