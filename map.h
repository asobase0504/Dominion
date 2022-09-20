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
	CMap();
	~CMap();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Set();
	void Load();

	void Delete();

	// Getter
	CBlock* GetBlock(int indexX, int indexY);
	std::vector<std::vector<CBlock*>>& GetBlockAll() { return m_block; }
	float GetBlockSize() { return BLOCK_SIZE; }
	int GetMaxXBlock() { return MAX_X_BLOCK; }
	int GetMaxYBlock() { return MAX_Y_BLOCK; }

	// Setter
	void SetMap(std::vector<std::vector<CBlock::BLOCK_TYPE>> inBlockIdx);

private:
	float BLOCK_SIZE;		// �u���b�N�̃T�C�Y
	static const float X_CENTER;	// �}�b�v�𒆉��ɔz�u���邽��(X��)
	static const float Y_CENTER;	// �}�b�v�𒆉��ɔz�u���邽��(Y��)
	int MAX_X_BLOCK;	// X���̍ő吔
	int MAX_Y_BLOCK;	// Y���̍ő吔
	std::vector<std::vector<CBlock*>> m_block;
	std::vector<std::vector<CBlock::BLOCK_TYPE>> m_blockIdx;
};

#endif // !_MAP_H_
