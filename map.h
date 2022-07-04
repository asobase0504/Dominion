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
#include "main.h"
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
private:
	static const int MAX_X_BLOCK = 40;	// X���̍ő吔
	static const int MAX_Y_BLOCK = 17;	// Y���̍ő吔
	static const float BLOCK_SIZE;	// �u���b�N�̃T�C�Y
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
	CBlock* GetBlock();
private:
	std::vector<CBlock*> m_block;
	CBlock::TYPE m_blockIdx[MAX_X_BLOCK][MAX_Y_BLOCK];
};

#endif // !_MAP_H_
