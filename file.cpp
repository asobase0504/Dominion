//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include <fstream>
#include "nlohmann/json.hpp"

#include "application.h"
#include "texture.h"

nlohmann::json j;//リストの生成

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
std::vector<std::vector<CBlock::BLOCK_TYPE>> LoadJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);
	std::vector<std::vector<CBlock::BLOCK_TYPE>> blockIdx;

	if (ifs)
	{
		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> j;

		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		blockIdx = j["MAP"];

		return blockIdx;
	}
	return blockIdx;
}

void LoadJsonTex(const char* cUrl)
{
	std::string name = "LINK";
	std::string Number = std::to_string(index);
	name += Number;
	TEX[name] = { { "LINK", cUrl } };
	
	index++;
}

void OutputStatusTex()
{
	TEX["Index"] = index;
	auto jobj = TEX.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/Textest.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void SetTex()
{
	const std::string pathToJSON = "data/FILE/Textest.json";
	std::ifstream ifs(pathToJSON);

	if (ifs)
	{
		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> TEX;
		index = TEX["Index"];
		for (int nCnt = 0; nCnt < index; nCnt++)
		{
			
			std::string name = "LINK";
			std::string Number = std::to_string(nCnt);
			name += Number;

			Lync[nCnt] = TEX[name]["LINK"];

			CTexture* pTexture = CApplication::GetInstance()->GetTextureClass();
		}
	}

	index = 0;
}