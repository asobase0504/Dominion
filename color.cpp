//=========================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "color.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CColor::CColor()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CColor::~CColor()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CColor::Init()
{
	//m_color.insert(std::make_pair("COLOR1", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR2", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR3", D3DXCOLOR(0.45f, 0.45f, 0.9f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR4", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));

	m_color.insert(std::make_pair("COLOR1", D3DXCOLOR(1.0f, 0.9f, 0.2f, 1.0f)));
	m_color.insert(std::make_pair("COLOR2", D3DXCOLOR(0.8f, 0.2f, 1.0f, 1.0f)));
	m_color.insert(std::make_pair("COLOR3", D3DXCOLOR(0.1f, 0.3f, 0.3f, 1.0f)));
	m_color.insert(std::make_pair("COLOR4", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));

	//m_color.insert(std::make_pair("COLOR1", D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR2", D3DXCOLOR(0.0f, 0.0f, 0.1f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR3", D3DXCOLOR(0.45f, 0.45f, 0.9f, 1.0f)));
	//m_color.insert(std::make_pair("COLOR4", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CColor::Uninit()
{
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CColor::Update()
{
}

//-----------------------------------------
// 描画
//-----------------------------------------
void CColor::Draw()
{
	// 処理なし
}

//-----------------------------------------
// 色の取得
//-----------------------------------------
D3DXCOLOR CColor::GetColor(std::string inConfig)
{
	// 指定されたKeyが存在するか否か
	if (m_color.count(inConfig) == 0)
	{ // Keyが設定されてなかった場合
		return D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	}

	/* ↓Keyが設定されていた場合↓ */

	return m_color[inConfig];
}
