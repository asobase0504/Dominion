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
#include <map>
#include <vector>
#include "character.h"

// �O���錾
class CBlock;

//�f�[�^�^��`
enum ASTAR_STATUS
{
	ASTAR_EMPTY,	// ���T��
	ASTAR_OPEN,		// �ŒZ���[�g���
	ASTAR_CLOSED,	// �ŒZ���[�g
	ASTAR_OBSTACLE,	// ��Q��
	ASTAR_GOAL,		// �S�[��
};

struct CELL
{
	POINT ptIndex;	// �ԍ�
	POINT ptParent;	// �e	
	int iCost;				// ���R�X�g
	int iHeuristic;			// ����R�X�g
	int iScore;				// ���v�X�R�A
	ASTAR_STATUS Status;	// �X�e�[�^�X��ێ�����ϐ�
	bool boRealPath;		// ���������������Ƃ��Ɍo�H��ɂ���Z���ɂ����āA���̃t���O��TRUE�ɂȂ�B�܂肱�̃t���O���P�̎��Z������ׂ�ƌo�H�ɂȂ�
};

struct ASTAR_PARAM	// �}�b�v���I�ȁH
{
	POINT ptStartPos;	// �X�^�[�g�̈ʒu
	POINT ptGoalPos;	// �S�[���̈ʒu
	POINT ptCurrentPos;		// ���ݒn
};

class ASTAR
{
public:
	//�R���X�g���N�^
	ASTAR();
	//�f�X�g���N�^
	~ASTAR();

	HRESULT Init(std::vector<std::vector<CBlock*>>& inStage, CCharacter::TEAM inTeam);
	void Uninit();
	HRESULT CalcPath(ASTAR_PARAM*);	//�p�X�v�Z�֐�

	// Getter
	std::vector<POINT> GetPath() { return m_path; }

	HRESULT CalcScore(ASTAR_PARAM*);	// �ŏd�v�֐����ꂪ��������I���	�i�X�R�A�̌v�Z�A�S�[���܂ł̃N���[�X�h���X�g�����j
	HRESULT MakePathFromClosedList(ASTAR_PARAM*);	// �ŏI�p�X�̐���
	int CalcDistance(POINT*, POINT*);	// �v���C���[�ƓG�̋��������߂Ă��邾��
	HRESULT Reset();	// �Č������\�ɂȂ邽��

private:
	int GetIndexToBlock(POINT inPoint);
	int GetIndexToBlock(int X, int Y);
private:
	std::vector<POINT> m_path;	// �ŏI�I�Ɍ��������ŒZ�o�H�Z���Q

	// �u���b�N���
	std::vector<std::vector<CBlock*>>* m_stage;	// �X�e�[�W���
	int m_team;
	std::vector<CELL> m_cell;	// �S�ẴZ�����

	std::vector<POINT> m_openList;		// �I�[�v�����X�g
	std::vector<POINT> m_closedList;	// �N���[�X���X�g

	POINT m_startIndex;
	POINT m_goalIndex;

	int m_widthSize;		// ���̃T�C�Y��
	int m_heightSize;		// �����̃T�C�Y��

	POINT m_ptVolute[5];	// ����8�������Q�Ƃ��邳���̗��֐��̂���	[1]����[2]��[3]�E��[4]���@	���������͑����@[5]����[6]��[7]�E��[8]�E
	POINT m_ptGoal;			// �S�[���̕ϐ�
};
#endif