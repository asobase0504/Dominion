//===========================================================
//
//�Q�l���@�Q�[���v���O���~���OC++�A�Q�[���J���̂��߂�AI����,��w�Z�ɉ�����z���O��ŏ��������Ƃ�
//
//�@Author ��]����
//
//===========================================================
#ifndef _ASTAR_H_
#define _ASTAR_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include <d3dx9.h>

//�f�[�^�^��`
enum ASTAR_STATUS
{
	ASTAR_EMPTY,	// ��
	ASTAR_OPEN,		// ���T��
	ASTAR_CLOSED,	// �T���ς�
	ASTAR_OBSTACLE,	// ��Q��
	ASTAR_GOAL,		// �S�[��
};

//�������g�̍��W
struct CELL
{
	D3DXVECTOR2 ptIndex;
	D3DXVECTOR2 ptParent;
	int iCost;
	int iHeuristic;
	int iScore;
	ASTAR_STATUS Status;	//�X�e�[�^�X��ێ�����ϐ�
	BOOL boRealPath;		//���������������Ƃ��Ɍo�H��ɂ���Z���ɂ����āA���̃t���O��TRUE�ɂȂ�B�܂肱�̃t���O���P�̎��Z������ׂ�ƌo�H�ɂȂ�
	VOID* ppv1;				//���b�V��		main.cpp�Ŏg�p����
};

struct ASTAR_INIT
{
	CELL* pCell;
	DWORD dwCellWidth;
	DWORD dwCellHeight;		
};

struct ASTAR_PARAM	// �}�b�v���I�ȁH
{
	CELL* pCell;	// �Z��
	D3DXVECTOR2 ptStartPos;	// �X�^�[�g�̈ʒu
	D3DXVECTOR2 ptGoalPos;	// �S�[���̈ʒu
	D3DXVECTOR2 ptCurrentPos;		//���ݒn
};

class ASTAR
{
public:
	//�R���X�g���N�^
	ASTAR();
	//�f�X�g���N�^
	~ASTAR();
	D3DXVECTOR2* m_ptPath;	//�ŏI�I�Ɍ��������ŒZ�o�H�Z���Q�ւ�pointer
	DWORD m_dwPathList;		//�ŏI�p�X���`������Z���̌�

	HRESULT Init(ASTAR_INIT*);
	HRESULT CalcPath(ASTAR_PARAM*);	//�p�X�v�Z�֐�
protected:
	CELL* m_pCell;				//���ׂẴZ���̐擪�Z��
	D3DXVECTOR2* m_pOpenList;	//�I�[�v�����X�g�̐擪pointer
	DWORD m_dwOpenAmt;			//�I�[�v�����X�g���̃Z���̐�
	D3DXVECTOR2* m_pClosedList;	//�N���[�X���X�g�̐擪pointer
	int m_dwClosedAmt;		//�N���[�X���X�g���̃Z���̐�

	DWORD m_dwCellWidth;		//�Z����
	DWORD m_dwCellHeight;		//�Z��������

	D3DXVECTOR2 m_ptVolute[9+1];	//����8�������Q�Ƃ��邳���̗��֐��̂���	[1]����[2]��[3]�E��[4]���@	���������͑����@[5]����[6]��[7]�E��[8]�E
	D3DXVECTOR2 m_ptGoal;			//�S�[���̕ϐ�

	HRESULT CalcScore(ASTAR_PARAM*);	//�ŏd�v�֐����ꂪ��������I���	�i�X�R�A�̌v�Z�A�S�[���܂ł̃N���[�X�h���X�g�����j
	HRESULT MakePathFromClosedList(ASTAR_PARAM*);	//�ŏI�p�X�̐���
	int CalcDistance(D3DXVECTOR2*, D3DXVECTOR2*);	//�v���C���[�ƓG�̋��������߂Ă��邾��
	HRESULT Reset();	//�Č������\�ɂȂ邽��
};
#endif