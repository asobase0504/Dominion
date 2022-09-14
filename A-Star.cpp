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


//�f�B�t�@�C���f���[�g
#define SAFE_DELETE(p)       { if(p) { delete (p);		(p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);	(p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release();	(p) = nullptr; } }

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"
#include "block.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author ��]����
//-----------------------------------------------------------------------------
ASTAR::ASTAR()
{	
	//������
	ZeroMemory(this,sizeof(ASTAR));

	//���W�̎���8�����@����8�Z���Ɉړ�����ۂɗ��p
	m_ptVolute[0].x=0;	//�ړ��Ȃ� 
	m_ptVolute[0].y=0;

	m_ptVolute[1].x=-1;	
	m_ptVolute[1].y=1;

	m_ptVolute[2].x=0;
	m_ptVolute[2].y=1;

	m_ptVolute[3].x=1;
	m_ptVolute[3].y=1;

	m_ptVolute[4].x=1;
	m_ptVolute[4].y=0;

	m_ptVolute[5].x=1;
	m_ptVolute[5].y=-1;

	m_ptVolute[6].x=0;
	m_ptVolute[6].y=-1;

	m_ptVolute[7].x=-1;
	m_ptVolute[7].y=-1;

	m_ptVolute[8].x=-1;
	m_ptVolute[8].y=0;
	
	/* �ԍ��ƍ��W�̑Ή�
	1 2 3
    8 0 4
    7 6 5
	*/	
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author ��]����
//-----------------------------------------------------------------------------
ASTAR::~ASTAR()
{
	SAFE_DELETE_ARRAY(m_pOpenList);
	SAFE_DELETE_ARRAY(m_pClosedList);
	SAFE_DELETE_ARRAY(m_ptPath);
}

//-----------------------------------------------------------------------------
// ������
// Author ��]����
// �����F�ŏI�I�ȃp�X���i�[����o�b�t�@�̃������m��
//-----------------------------------------------------------------------------
HRESULT ASTAR::Init(ASTAR_INIT* pai)
{
	//
	m_widthSize = pai->dwCellWidth;
	m_heightSize = pai->dwCellHeight;
	m_pCell = pai->pCell;

	//���I�m��
	m_pOpenList = (POINT*) new POINT[m_widthSize * m_heightSize];
	if(!m_pOpenList)
	{//�������s���̃G���[
		return E_OUTOFMEMORY;
	}

	m_pClosedList = (POINT*) new POINT[m_widthSize * m_heightSize];
	if(!m_pClosedList)
	{//�������s���̃G���[
		return E_OUTOFMEMORY;
	}

	m_ptPath = (POINT*) new POINT[m_widthSize * m_heightSize];
	if(!m_ptPath)
	{//�������s���̃G���[
		return E_OUTOFMEMORY;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// ���Z�b�g
// Author ��]����
//-----------------------------------------------------------------------------
HRESULT ASTAR::Reset()
{
	int dwMaxAmt = m_widthSize * m_heightSize;

	for (int i = 0; i < dwMaxAmt; i++)
	{
		m_pCell[i].iCost = 0;
		m_pCell[i].iHeuristic = 0;
		m_pCell[i].iScore = 0;
		m_pCell[i].boRealPath = FALSE;
		ZeroMemory(&m_pCell[i].ptParent, sizeof(D3DXVECTOR2));
		if (m_pCell[i].Status != ASTAR_OBSTACLE)
		{
			m_pCell[i].Status = ASTAR_EMPTY;
		}
	}

	m_dwOpenAmt = 0;
	m_dwClosedAmt = 0;
	m_dwPathList = 0;
	ZeroMemory(m_ptPath, sizeof(D3DXVECTOR2) * m_widthSize * m_heightSize);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Author ��]����
// �����FHRESULT�^�͐����������ǂ�����Ԃ��Ă���
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	int idx = (int)pParam->ptGoalPos.y * m_widthSize + (int)pParam->ptGoalPos.x;	// �S�[���Ɏw�肳�ꂽ�Z���̔ԍ�

	//�S�[�����K���ȏꏊ���i�S�[������Q���̓����ɂȂ��Ă��Ȃ����j
	if (m_pCell[idx].Status == ASTAR_OBSTACLE)
	{
		MessageBox(0, "obstacle", "", MB_OK);	// MB_OK�̓e�L�X�g�{�b�N�X��ok�{�^�����o�����邽�߂̈���
		return E_FAIL;	// ���s�����Ȃ�S_OK�o�Ȃ�E_FAIL��Ԃ����ɂȂ�G���[��f��
	}

	/* ���S�[�����K�؂������ꍇ�� */

	Reset();	// ���Z�b�g

	// A*�ł̒���
	if (SUCCEEDED(CalcScore(pParam)))
	{
		MakePathFromClosedList(pParam);
	}
	else
	{
		MessageBox(0, "�����Ȃ�", "", MB_OK);
	}
	return S_OK;	// ���������Ƃ���S_OK��Ԃ�
}

//-----------------------------------------------------------------------------
// �X�e�[�W���̐ݒ�
// Author ���c �C�s
//-----------------------------------------------------------------------------
void ASTAR::SetCellStage(std::vector<std::vector<CBlock*>>& inStage , CCharacter::TEAM inTeam)
{
	m_stage = &inStage;

	for (int i = 0; i < (int)m_stage->size(); i++)
	{
		for (int j = 0; j < (int)m_stage->at(i).size(); j++)
		{
			CELL cell;
			cell.ptIndex;			// �ԍ�
			cell.ptParent;			// �e	
			cell.iCost = 0;			// ���R�X�g
			cell.iHeuristic = 0;	// ����R�X�g
			cell.iScore = 0;		// ���v�X�R�A

			// �����`�[�����ۂ�
			bool isSameTeam = (int)inTeam == (int)m_stage->at(i).at(j)->CBlock::GetType();

			cell.Status = isSameTeam ? ASTAR_EMPTY : ASTAR_OBSTACLE;	// �X�e�[�^�X��ݒ肷��

			cell.boRealPath = false;	// �o�H�̏��̏�����

			// �Z���̒ǉ�
			m_cell.insert(std::make_pair(&m_stage->at(i).at(j), cell));
		}
	}
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

	// ���ݒn�̃Z���͖������ŃN���[�Y�h�ɂ���
	CBlock* thisBlock = m_stage->at(ptCurrentPos.x).at(ptCurrentPos.y);
	CELL* cell = &m_cell[&thisBlock];	// �Z�����

	if (cell->Status != ASTAR_CLOSED)
	{
		cell->Status = ASTAR_CLOSED;
		m_dwClosedAmt++;
	}

	int iOffset;

	// �א�8�Z���̃X�R�A�����O�@�i�߂�}�X��������
	for (int i = 1; i <= 8; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		//�Z���}�b�v�͈̔͊O�ɓ��B�����ꍇ
		if ((ptCurrentPos.x < 0) || (ptCurrentPos.x > m_widthSize) || (ptCurrentPos.y < 0) || (ptCurrentPos.y >m_heightSize))
		{
			continue;	//�������X�L�b�v����
		}

		/* �����ݒn���Z���}�b�v�͈͓̔��������ꍇ�� */

		iOffset = ptCurrentPos.x * m_heightSize + ptCurrentPos.y;
		thisBlock = m_stage->at(ptCurrentPos.x).at(ptCurrentPos.y);

		//�S�[���Z���ɓ��B�����ꍇ
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			cell->ptParent = pParam->ptCurrentPos;//�e�Z���ւ̃����N
			cell->Status = ASTAR_GOAL;
			m_ptGoal = cell->ptIndex;
			return S_OK;
		}

		/* ���܂��S�[���ł͂Ȃ��ꍇ�� */

		switch (cell->Status)
		{
		case ASTAR_EMPTY:	//�i�ނƂ�
			cell->iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			//�R�X�g�͐e�̃R�X�g�ɂP�𑫂�������
			{
				cell->iCost = m_pCell[pParam->ptCurrentPos.x * m_heightSize + pParam->ptCurrentPos.y].iCost + 1;
			}
			cell->iScore = cell->iCost + cell->iHeuristic;
			cell->Status = ASTAR_OPEN;
			m_pOpenList[m_dwOpenAmt] = ptCurrentPos;
			m_dwOpenAmt++;
			//�W�Z���̐e�����݃Z���ɂ���
			{
				cell->ptParent = pParam->ptCurrentPos;//
			}
			break;
		case ASTAR_OPEN:
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			continue;
			break;
		}
	}

	/* ���V����CELL���烋�[�g�������s���� */

	//�I�[�v�����X�g���܂߂čŏ��X�R�A���A�V���ȋN�_�Z���ɂ���@�i�N���[�Y�h���X�g�̍쐬�j
	int iMinScore = INT_MAX;
	POINT ptNextCellIndex = { 0 ,0 };
	DWORD dwMinIndex = 0;

	// �X�R�A���ŏ��̃Z����T��
	for (int i = 0; i < m_dwOpenAmt; i++)
	{
		thisBlock = m_stage->at(m_pOpenList[i].x).at(m_pOpenList[i].y);

		iOffset = m_pOpenList[i].x * m_heightSize + m_pOpenList[i].y;

		if (m_pCell[iOffset].iScore < iMinScore && m_pCell[iOffset].Status == ASTAR_OPEN)
		{
			iMinScore = m_pCell[iOffset].iScore;
			dwMinIndex = i;
		}
	}

	thisBlock = m_stage->at(m_pOpenList[dwMinIndex].x).at(m_pOpenList[dwMinIndex].y);
	iOffset = m_pOpenList[dwMinIndex].x * m_heightSize + m_pOpenList[dwMinIndex].y;
	ptNextCellIndex = m_pCell[iOffset].ptIndex;

	m_pClosedList[m_dwClosedAmt] = ptNextCellIndex;

	//�V���ȋN�_�Z���ɂ��ċA�Ăяo��
	ASTAR_PARAM Param;
	Param.ptStartPos = pParam->ptStartPos;
	Param.ptGoalPos = pParam->ptGoalPos;
	Param.ptCurrentPos = m_pOpenList[dwMinIndex];
	Param.pCell = pParam->pCell;

	return CalcScore(&Param);
}

//-----------------------------------------------------------------------------
// Author ��]����
//-----------------------------------------------------------------------------
HRESULT ASTAR::MakePathFromClosedList(ASTAR_PARAM* pParam)
{
	int iBreak = 0;
	int iMaxStep = m_widthSize * m_heightSize;	//�����̃}�X
	m_dwPathList = 0;
	// �N���[�Y�h�Z����ŁA�S�[���n�_����X�^�[�g�n�_�܂Őe��H�����o�H���ŏI�I�ȃp�X�Ƃ���

	int iOffset = m_ptGoal.x*m_heightSize + m_ptGoal.y;	// �ԍ���ID

	//while�ƈႢ�P��͕K���ʂ�
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "�e�̃����N���X�^�[�g�n�_�܂Ōq�����Ă��Ȃ����A�����N���z���Ă��܂�", "", MB_OK);
			break;
		}
		iBreak++;
		m_pCell[iOffset].boRealPath = true;

		iOffset = m_pCell[iOffset].ptParent.x * m_heightSize + m_pCell[iOffset].ptParent.y;

		//�p�X���X�g�ɋL�^�i�Ăяo�����̗��֐�)	�N���A�����O
		m_ptPath[m_dwPathList] = m_pCell[iOffset].ptIndex;
		m_dwPathList++;
	} while (m_pCell[iOffset].ptIndex.x != pParam->ptStartPos.x || m_pCell[iOffset].ptIndex.y != pParam->ptStartPos.y);

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
	return (int)sqrtf((float)iX*iX + (float)iY*iY);
}