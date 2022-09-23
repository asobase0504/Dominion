//=============================================================================
//
// �Q�l���@�Q�[���v���O���~���OC++�A�Q�[���J���̂��߂�AI����,��w�Z�ɉ�����z���O��ŏ��������Ƃ�
// Author ��]����
//
//=============================================================================
// �T�v�F
/*
	���D��T���͎n�_����ϓ��ɒT�����邩�����������
	�ŗǗD��T���̓S�[���ɋ߂����_���ŗǂƂ��Ă���(�����)
	A*�� �ŗǗD��T���̉��ǔŁ@���_�܂ł̋����@+�@���_�܂ł̋���	�Ōv�Z�@
	�Ȃ�A*�Ȃ̂��H	Q&A ���_�܂ł̋����@+�@���_�܂ł̋����Ȃ̂������ [�܏��H] ����E�o���₷�����邽��
	�撣���Đ������Ă݂����Ǖ�����Ȃ������璲�ׂĂ݂Ă���
*/

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"
#include "block.h"
#include <assert.h>

#include "utility.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author ��]����
//-----------------------------------------------------------------------------
ASTAR::ASTAR() :
	m_stage(nullptr),
	m_team(0),
	m_startIndex({0,0}),
	m_goalIndex({0,0}),
	m_widthSize(0),
	m_heightSize(0),
	m_ptGoal({ 0,0 })
{
	m_path.clear();
	m_cell.clear();
	m_openList.clear();
	m_closedList.clear();
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author ��]����
//-----------------------------------------------------------------------------
ASTAR::~ASTAR()
{
	assert(m_openList.empty());
	assert(m_closedList.empty());
}

//-----------------------------------------------------------------------------
// ������
// Author ��]����
// �����F�ŏI�I�ȃp�X���i�[����o�b�t�@�̃������m��
//-----------------------------------------------------------------------------
HRESULT ASTAR::Init(std::vector<std::vector<CBlock*>>& inStage, CCharacter::TEAM inTeam)
{
	//���W�̎���8�����@����8�Z���Ɉړ�����ۂɗ��p
	/* �ԍ��ƍ��W�̑Ή�
	  1  
	4 0 1
	  3  
	*/
	m_ptVolute[0].x = 0;	//�ړ��Ȃ� 
	m_ptVolute[0].y = 0;

	m_ptVolute[1].x = 0;
	m_ptVolute[1].y = 1;

	m_ptVolute[2].x = 1;
	m_ptVolute[2].y = 0;

	m_ptVolute[3].x = 0;
	m_ptVolute[3].y = -1;

	m_ptVolute[4].x = -1;
	m_ptVolute[4].y = 0;

	m_stage = &inStage;
	m_team = inTeam;

	// �傫���̎擾
	m_widthSize = m_stage->at(0).size();
	m_heightSize = m_stage->size();

	m_openList.clear();
	m_closedList.clear();

	//Reset();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
// Author ���c�C�s
//-----------------------------------------------------------------------------
void ASTAR::Uninit()
{
	m_stage = nullptr;
	m_path.clear();
	m_cell.clear();
	m_openList.clear();
	m_closedList.clear();
}

//-----------------------------------------------------------------------------
// ���Z�b�g
// Author ��]����
//-----------------------------------------------------------------------------
HRESULT ASTAR::Reset()
{
	m_cell.clear();
	for (int y = 0; y < (int)m_stage->size(); y++)
	{
		for (int x = 0; x < (int)m_stage->at(y).size(); x++)
		{
			CELL cell;
			cell.ptIndex.x = x;		// �ԍ�
			cell.ptIndex.y = y;		// �ԍ�
			cell.ptParent.x = 0;	// �e	
			cell.ptParent.y = 0;	// �e	
			cell.iCost = 1;			// ���R�X�g

			int distX = m_startIndex.x - cell.ptIndex.x;
			int distY = m_startIndex.y - cell.ptIndex.y;

			if (distX >= distY)
			{
				cell.iHeuristic = distX;	// ����R�X�g
			}
			else
			{
				cell.iHeuristic = distY;	// ����R�X�g
			}

			cell.iScore = 0;		// ���v�X�R�A

			// �����`�[�����ۂ�
			bool isSameTeam = (int)m_team == (int)m_stage->at(y).at(x)->CBlock::GetType();
			cell.Status = isSameTeam ? ASTAR_EMPTY : ASTAR_OBSTACLE;	// �X�e�[�^�X��ݒ肷��

			cell.boRealPath = false;	// �o�H�̏��̏�����

			// �Z���̒ǉ�
			m_cell.push_back(cell);
		}
	}

	m_openList.clear();
	m_closedList.clear();
	m_path.clear();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �ԍ�����u���b�N���擾
// Author ���c�C�s
//-----------------------------------------------------------------------------
int ASTAR::GetIndexToBlock(POINT inPoint)
{
	if (m_stage == nullptr)
	{
		assert(false);
	}

	if ((int)m_stage->size() <= inPoint.y)
	{
		return m_stage->at(m_stage->size() - 1).size() * m_stage->size() - 1 + inPoint.x;
	}

	return m_stage->at(inPoint.y).size() * inPoint.y + inPoint.x;
}

//-----------------------------------------------------------------------------
// �ԍ�����u���b�N���擾
// Author ���c�C�s
//-----------------------------------------------------------------------------
int ASTAR::GetIndexToBlock(int X, int Y)
{
	return 	m_stage->at(Y).size() * Y + X;
}

//-----------------------------------------------------------------------------
// Author ��]����
// �����FHRESULT�^�͐����������ǂ�����Ԃ��Ă���
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	m_startIndex = pParam->ptStartPos;
	m_goalIndex = pParam->ptGoalPos;

	Reset();	// ���Z�b�g

	// �S�[�����K���ȏꏊ���i�S�[������Q���̓����ɂȂ��Ă��Ȃ����j
	if (m_cell.at(GetIndexToBlock(pParam->ptGoalPos)).Status == ASTAR_OBSTACLE)
	{
		DEBUG_PRINT("�S�[������Q���ɂȂ��Ă܂�\n");
		return E_FAIL;	// ���s�����Ȃ�S_OK�o�Ȃ�E_FAIL��Ԃ����ɂȂ�G���[��f��
	}

	/* ���S�[�����K�؂������ꍇ�� */

	// A*�ł̒���
	if (SUCCEEDED(CalcScore(pParam)))
	{
		MakePathFromClosedList(pParam);
	}
	else
	{
		DEBUG_PRINT("���[�g�\�z���o���܂���\n");
		return E_FAIL;
	}
	return S_OK;	// ���������Ƃ���S_OK��Ԃ�
}

//-----------------------------------------------------------------------------
// �P��̌Ăяo����8�����Z���̒������s��
// Author ��]����
// �����F�ċA���Ȃ���S�[���܂ł̌v�Z�����ׂčs��
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcScore(ASTAR_PARAM* pParam)
{
	POINT ptStart = pParam->ptStartPos;			// �X�^�[�g�ʒu
	POINT ptGoal = pParam->ptGoalPos;			// �S�[���ʒu
	POINT ptCurrentPos = pParam->ptCurrentPos;	// ���ݒn
	int end = 0;

	// ���ݒn�̃Z���͖������ŃN���[�Y�h�ɂ���
	CELL* cell = &m_cell.at(GetIndexToBlock(ptCurrentPos));	// �Z�����

	if (cell->Status == ASTAR_CLOSED)
	{
		return E_FAIL;
	}

	if (cell->Status != ASTAR_CLOSED)
	{
		cell->Status = ASTAR_CLOSED;
		m_closedList.push_back(cell->ptIndex);
	}

	// �א�8�Z���̃X�R�A�����O�@�i�߂�}�X��������
	for (int i = 1; i <= 4; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		cell = &m_cell.at(GetIndexToBlock(ptCurrentPos));

		// �Z���}�b�v�͈̔͊O�ɓ��B�����ꍇ
		if ((ptCurrentPos.x < 0) || (ptCurrentPos.x > m_widthSize) || (ptCurrentPos.y < 0) || (ptCurrentPos.y > m_heightSize))
		{
			continue;	// �������X�L�b�v����
		}

		/* �����ݒn���Z���}�b�v�͈͓̔��������ꍇ�� */

		// �S�[���Z���ɓ��B�����ꍇ
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			cell->ptParent = pParam->ptCurrentPos;	// �e�Z���ւ̃����N
			cell->Status = ASTAR_GOAL;
			m_ptGoal = cell->ptIndex;
			return S_OK;
		}

		/* ���܂��S�[���ł͂Ȃ��ꍇ�� */

		switch (cell->Status)
		{
		case ASTAR_EMPTY:	// �i�ނƂ�
			//cell->iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			// �R�X�g�͐e�̃R�X�g�ɂP�𑫂�������
			{
				cell->iCost = m_cell.at(GetIndexToBlock(pParam->ptCurrentPos)).iCost + 1;
			}
			cell->iScore = cell->iCost + cell->iHeuristic;
			cell->Status = ASTAR_OPEN;
			m_openList.push_back(ptCurrentPos);
			// 4�Z���̐e�����݃Z���ɂ���
			{
				cell->ptParent = pParam->ptCurrentPos;	// 
			}
			break;
		case ASTAR_OPEN:
			break;
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			end++;
			continue;
			break;
		}
	}

	/* ���V����CELL���烋�[�g�������s���� */

	// �I�[�v�����X�g���܂߂čŏ��X�R�A���A�V���ȋN�_�Z���ɂ���@�i�N���[�Y�h���X�g�̍쐬�j
	int iMinScore = INT_MAX;
	DWORD dwMinIndex = 0;

	// �X�R�A���ŏ��̃Z����T��
	for (int i = 0; i < (int)m_openList.size(); i++)
	{
		CELL minCell = m_cell.at(GetIndexToBlock(m_openList.at(i)));

		if (minCell.iScore < iMinScore && minCell.Status == ASTAR_OPEN)
		{
			iMinScore = minCell.iScore;
			dwMinIndex = i;
		}
	}

	// �V���ȋN�_�Z���ɂ��ċA�Ăяo��
	ASTAR_PARAM Param;
	Param.ptStartPos = pParam->ptStartPos;
	Param.ptGoalPos = pParam->ptGoalPos;
	Param.ptCurrentPos = m_openList.at(dwMinIndex);

	return CalcScore(&Param);
}

//-----------------------------------------------------------------------------
// Author ��]����
//-----------------------------------------------------------------------------
HRESULT ASTAR::MakePathFromClosedList(ASTAR_PARAM* pParam)
{
	int iBreak = 0;
	int iMaxStep = m_widthSize * m_heightSize;	// �����̃}�X

	// �N���[�Y�h�Z����ŁA�S�[���n�_����X�^�[�g�n�_�܂Őe��H�����o�H���ŏI�I�ȃp�X�Ƃ���

	POINT iOffset;	// �ԍ���ID
	iOffset.x = m_ptGoal.x;
	iOffset.y = m_ptGoal.y;

	m_path.push_back(m_cell.at(GetIndexToBlock(iOffset)).ptIndex);

	// while�ƈႢ�P��͕K���ʂ�
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "�e�̃����N���X�^�[�g�n�_�܂Ōq�����Ă��Ȃ����A�����N���z���Ă��܂�", "", MB_OK);
			return E_FAIL;
			break;
		}
		iBreak++;

		POINT parentPoint = m_cell.at(GetIndexToBlock(iOffset)).ptParent;
		m_cell.at(GetIndexToBlock(parentPoint)).boRealPath = true;
		
		// �p�X���X�g�ɋL�^�i�Ăяo�����̗��֐�)	�N���A�����O
		m_path.push_back(m_cell.at(GetIndexToBlock(parentPoint)).ptIndex);

		iOffset = parentPoint;

	} while (m_cell.at(GetIndexToBlock(iOffset)).ptIndex.x != pParam->ptStartPos.x || m_cell.at(GetIndexToBlock(iOffset)).ptIndex.y != pParam->ptStartPos.y);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �����̎Z�o
// Author ��]����
//-----------------------------------------------------------------------------
int ASTAR::CalcDistance(POINT* pptA, POINT* pptB)
{
	int iX = pptA->x - pptB->x;
	int iY = pptA->y - pptB->y;
	return (int)sqrtf((float)iX * iX + (float)iY * iY);
}