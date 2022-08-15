//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include <fstream>

#include "application.h"
#include "texture.h"
#include "game.h"
#include "map.h"
#include "character.h"

nlohmann::json stage;//リストの生成

nlohmann::json TEX;//リストの生成

std::string Lync[256];
int index = 0;

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
}

//============================
// 読込み
//============================
nlohmann::json LoadJsonStage(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		ifs >> stage;

		return stage;
	}
	return nullptr;
}
