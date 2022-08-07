//==================================================
// 
// �f�o�b�O�@�\(�w�b�_�[�t�@�C��)
// Author : Yuda Kaito
// 
//==================================================
#ifndef _MAP_H_
#define _MAP_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include "block.h"
#include <vector>

//--------------------------------------------------
// �O���錾
//--------------------------------------------------
class CBlock;

//--------------------------------------------------
// �}�b�v�N���X
//--------------------------------------------------
class CMap
{
public:
	int MAX_X_BLOCK;	// X���̍ő吔
	int MAX_Y_BLOCK;	// Y���̍ő吔
	static float BLOCK_SIZE;		// �u���b�N�̃T�C�Y
	static const char* FILE_NAME;	// �}�b�v�t�@�C����
	static const float X_CENTER;	// �}�b�v�𒆉��ɔz�u���邽��(X��)
	static const float Y_CENTER;	// �}�b�v�𒆉��ɔz�u���邽��(Y��)

public:
	CMap();
	~CMap();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Set();
	void Load();
	CBlock* GetBlock(int indexX, int indexY);
	void SetMap(std::vector<std::vector<CBlock::BLOCK_TYPE>> inBlockIdx) { m_blockIdx = inBlockIdx; }

private:
	std::vector<std::vector<CBlock*>> m_block;
	std::vector<std::vector<CBlock::BLOCK_TYPE>> m_blockIdx;
};

#endif // !_MAP_H_
