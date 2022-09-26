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
	m_isPopFlag(false),
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
HRESULT CStage::Init(nlohmann::json inStage)
{
	// �}�b�v�N���X
	stage = inStage;

	map = new CMap;
	if (FAILED(map->Init()))
	{
		return E_FAIL;
	}
	map->SetMap(stage["MAP"]);
	map->Set();

	m_isPopFlag = true;
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
	// ��x�����ʂ�Ȃ��v���C���[�̍쐬
	PassOnceCreatePlater();

	// �L�����N�^�[�����S�������𒲂ׂ�
	for (int i = 0; i < (int)character.size(); i++)
	{
		if (!character[i]->GetIsDeleted())
		{
			continue;
		}

		/* �����S������ */
		m_isEndGame = true;
	}

	if (m_isEndGame)
	{
		// ����łȂ��L�����N�^�[�𒲂ׂ�
		for (int i = 0; i < (int)character.size(); i++)
		{
			character[i]->SetOperationState(false);
			if (character[i]->GetIsDeleted())
			{
				continue;
			}

			m_winnarTeam = character[i]->GetTeam();	// �L�����N�^�[��Idx���擾
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
// ��x�����ʂ�Ȃ�����
//-----------------------------------------------------------------------------
void CStage::PassOnceCreatePlater()
{
	if (!m_isPopFlag)
	{
		return;
	}

	if (m_PreparingCount <= 30)
	{
		m_PreparingCount++;
		return;
	}

	/* ���J�E���g�_�E�����I�������ꍇ�� */

	// �v���C���[�������_���ŃN���G�C�g
	auto playerSet = [this](int inIdx, CController* inController)->void
	{
		controller.push_back(inController);
		int x = stage["PLAYERS"].at(inIdx)["SPAWN"].at(0);				// X�̈ʒu�ԍ����擾
		int y = stage["PLAYERS"].at(inIdx)["SPAWN"].at(1);				// Y�̈ʒu�ԍ����擾
		CCharacter::TEAM inTeam = stage["PLAYERS"].at(inIdx)["TYPE"];	// �`�[���̍쐬
		character.push_back(CCharacter::Create(inTeam));				// ����
		character.at(inIdx)->SetStage(this);
		float size = map->GetBlockSize() * 0.65f;						// �傫���̐ݒ�
		character.at(inIdx)->SetSize(D3DXVECTOR2(size, size));			// �傫���̑��
		D3DXVECTOR3 pos = map->GetBlock(x, y)->GetPos();				// �ʒu�̎擾
		character.at(inIdx)->SetPos(pos);								// �ʒu�̐ݒ�
		character.at(inIdx)->SetCenterBlockIndex({ x,y });				// �����ʒu�̐ݒ�
		character.at(inIdx)->SetBlockIndex(0, { x,y });					// �����u���b�N��ݒ�
		character.at(inIdx)->SetController(inController);				// ���ߎ҂̐ݒ�
	};

	// �L�����N�^�[�̐ݒ�
	for (int i = 0; i < (int)m_controllerIndex.size(); i++)
	{
		if (m_controllerIndex[i] != -2)
		{
			CPlayerController* incontroller = new CPlayerController(m_controllerIndex[i]);
			playerSet(i, incontroller);
			incontroller->Init();
		}
		else
		{
			CAIController* incontroller = new CAIController;
			playerSet(i, incontroller);
			incontroller->Init();
		}
	}

	m_isPopFlag = false;
}

//-----------------------------------------------------------------------------
// �e�̍폜�˗�
//-----------------------------------------------------------------------------
void CStage::DeleteBullet()
{
	// �I�u�W�F�N�g�S�̂��擾
	for (auto it = CObject::GetMyObject(3)->begin(); it != CObject::GetMyObject(3)->end(); it++)
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

//-----------------------------------------------------------------------------
// ���E���h���҂̔ԍ����擾
//-----------------------------------------------------------------------------
int CStage::GetWinnerIndex()
{
	if (!m_isEndGame)
	{
		// �܂��Q�[�����I����ĂȂ��ꍇ
		assert(false);
		return -1;
	}

	return m_winnarTeam;
}
