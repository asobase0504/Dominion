//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include <fstream>

nlohmann::json stage;//リストの生成

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
