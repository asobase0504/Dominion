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
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"

//Thing[1]			�u���b�N
//Thing[2]			�ǂ������鑤
//Thing[3]			�ǂ��������鑤

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
	m_dwCellWidth = pai->dwCellWidth;
	m_dwCellHeight = pai->dwCellHeight;
	m_pCell = pai->pCell;

	//���I�m��
	m_pOpenList = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
	if(!m_pOpenList)
	{//�������s���̃G���[
		return E_OUTOFMEMORY;
	}

	m_pClosedList = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
	if(!m_pClosedList)
	{//�������s���̃G���[
		return E_OUTOFMEMORY;
	}

	m_ptPath = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
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
	DWORD dwMaxAmt = m_dwCellWidth * m_dwCellHeight;

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
	ZeroMemory(m_ptPath, sizeof(D3DXVECTOR2) * m_dwCellWidth * m_dwCellHeight);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Author ��]����
// �����FHRESULT�^�͐����������ǂ�����Ԃ��Ă���
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	int idx = (int)pParam->ptGoalPos.y * m_dwCellWidth + (int)pParam->ptGoalPos.x;	// �S�[���Ɏw�肳�ꂽ�Z���̔ԍ�

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
// �P��̌Ăяo����8�����Z���̒������s��
// Author ��]����
// �����F�ċA���Ȃ���S�[���܂ł̌v�Z�����ׂčs��
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcScore(ASTAR_PARAM* pParam)
{
	int iOffset;

	D3DXVECTOR2 ptStart = pParam->ptStartPos;	// �X�^�[�g�ʒu
	D3DXVECTOR2 ptGoal = pParam->ptGoalPos;		// �S�[���ʒu
	D3DXVECTOR2 ptCurrentPos = pParam->ptCurrentPos;	// ���ݒn

	// ���ݒn�̃Z���͖������ŃN���[�Y�h�ɂ���
	iOffset = ptCurrentPos.x * m_dwCellHeight + ptCurrentPos.y;

	if (m_pCell[iOffset].Status != ASTAR_CLOSED)
	{
		m_pCell[iOffset].Status = ASTAR_CLOSED;
		m_dwClosedAmt++;
	}

	// �א�8�Z���̃X�R�A�����O�@�i�߂�}�X��������
	for (int i = 1; i <= 8; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		//�Z���}�b�v�͈̔͊O�ɓ��B�����ꍇ
		if ((ptCurrentPos.x < 0) || ((DWORD)ptCurrentPos.x > m_dwCellWidth) || (ptCurrentPos.y < 0) || ((DWORD)ptCurrentPos.y > m_dwCellHeight))
		{
			continue;	//�������X�L�b�v����
		}

		/* �����ݒn���Z���}�b�v�͈͓̔��������ꍇ�� */

		iOffset = ptCurrentPos.x * m_dwCellHeight + ptCurrentPos.y;

		//�S�[���Z���ɓ��B�����ꍇ
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			m_pCell[iOffset].ptParent = pParam->ptCurrentPos;//�e�Z���ւ̃����N
			m_pCell[iOffset].Status = ASTAR_GOAL;
			m_ptGoal = m_pCell[iOffset].ptIndex;
			return S_OK;
		}

		/* ���܂��S�[���ł͂Ȃ��ꍇ�� */

		switch (m_pCell[iOffset].Status)
		{
		case ASTAR_EMPTY:	//�i�ނƂ�
			m_pCell[iOffset].iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			//�R�X�g�͐e�̃R�X�g�ɂP�𑫂�������
			{
				m_pCell[iOffset].iCost = m_pCell[(int)pParam->ptCurrentPos.x * m_dwCellHeight + (int)pParam->ptCurrentPos.y].iCost + 1;
			}
			m_pCell[iOffset].iScore = m_pCell[iOffset].iCost + m_pCell[iOffset].iHeuristic;
			m_pCell[iOffset].Status = ASTAR_OPEN;
			m_pOpenList[m_dwOpenAmt] = ptCurrentPos;
			m_dwOpenAmt++;
			//�W�Z���̐e�����݃Z���ɂ���
			{
				m_pCell[iOffset].ptParent = pParam->ptCurrentPos;//
			}
			break;
		case ASTAR_OPEN:
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			continue;
			break;
		}
	}

	//�I�[�v�����X�g���܂߂čŏ��X�R�A���A�V���ȋN�_�Z���ɂ���@�i�N���[�Y�h���X�g�̍쐬�j
	int iMinScore = INT_MAX;
	D3DXVECTOR2 ptNextCellIndex = { 0 ,0 };
	DWORD dwMinIndex = 0;

	for (int i = 0; i < m_dwOpenAmt; i++)
	{
		iOffset = m_pOpenList[i].x * m_dwCellHeight + m_pOpenList[i].y;

		if (m_pCell[iOffset].iScore < iMinScore && m_pCell[iOffset].Status == ASTAR_OPEN)
		{
			iMinScore = m_pCell[iOffset].iScore;
			dwMinIndex = i;
		}
	}

	iOffset = m_pOpenList[dwMinIndex].x * m_dwCellHeight + m_pOpenList[dwMinIndex].y;
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
	int iMaxStep = m_dwCellWidth * m_dwCellHeight;	//�����̃}�X
	m_dwPathList = 0;
	// �N���[�Y�h�Z����ŁA�S�[���n�_����X�^�[�g�n�_�܂Őe��H�����o�H���ŏI�I�ȃp�X�Ƃ���

	int iOffset = m_ptGoal.x*m_dwCellHeight + m_ptGoal.y;	// �ԍ���ID

	//while�ƈႢ�P��͕K���ʂ�
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "�e�̃����N���X�^�[�g�n�_�܂Ōq�����Ă��Ȃ����A�����N���z���Ă��܂�", "", MB_OK);
			break;
		}
		iBreak++;
		m_pCell[iOffset].boRealPath = TRUE;
		iOffset = m_pCell[iOffset].ptParent.x*m_dwCellHeight + m_pCell[iOffset].ptParent.y;
		//�p�X���X�g�ɋL�^�i�Ăяo�����̗��֐�)	�N���A�����O
		m_ptPath[m_dwPathList] = m_pCell[iOffset].ptIndex;
		m_dwPathList++;
	} while (m_pCell[iOffset].ptIndex.x != pParam->ptStartPos.x || m_pCell[iOffset].ptIndex.y != pParam->ptStartPos.y);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �ړI�ƍŏI�n�̓���
// Author ��]����
//-----------------------------------------------------------------------------
int ASTAR::CalcDistance(D3DXVECTOR2* pptA, D3DXVECTOR2* pptB)
{
	int iX = pptA->x - pptB->x;
	int iY = pptA->y - pptB->y;
	return (int)sqrtf((float)iX*iX + (float)iY*iY);
}