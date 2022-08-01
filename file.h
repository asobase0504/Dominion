//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_	// このマクロ定義がされてなかったら
#define _FILE_H_	// 二重インクルード防止のマクロ定義

#include "block.h"
#include "nlohmann/json.hpp"

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数
void OutputStatus();
nlohmann::json LoadJsonStage(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
#endif
