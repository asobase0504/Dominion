//============================
//
// �t�@�C���ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_	// ���̃}�N����`������ĂȂ�������
#define _FILE_H_	// ��d�C���N���[�h�h�~�̃}�N����`

#include "block.h"
#include <vector>

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
void OutputStatus();
std::vector<std::vector<CBlock::BLOCK_TYPE>> LoadJson(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
#endif
