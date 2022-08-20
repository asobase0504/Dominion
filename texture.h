//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include <vector>
#include <map>
#include <string>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_PLAYER = 0,	// プレイヤーテクスチャ
		TEXTURE_BLOCK,		// ブロック
		TEXTURE_BULLET,		// 残弾
		TEXTERE_BG,			// 背景
		TEXTURE_MAX,
		TEXTURE_NONE,	// 使用しない
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();					// 全ての読み込み
	void UnloadAll();				// 全ての破棄
	void LoadInVector(std::vector<std::string> inTexture);	// 指定の読み込み
	void Unload(TEXTURE inTexture);	// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(std::string inTexture);	// 情報の取得

private: /* メンバ変数 */
	std::map<std::string, LPDIRECT3DTEXTURE9, std::less<>> m_texture;	// テクスチャの情報
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
