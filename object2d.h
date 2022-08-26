//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "object.h"
#include "texture.h"
#include <string>

class CObject2D : public CObject
{
public:

	enum Anchor
	{
		CENTER = 0,
		TOP,
		DOWN,
		LEFT,
		RIGHT,
		TOPLEFT,
		TOPRIGHT,
		DOWNLEFT,
		DOWNRIGHT,
	};

	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	// 頂点フォーマット
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

public:	// 静的関数
	static CObject2D* Create();

public:
	CObject2D(TYPE type = TYPE::NONE);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetAnchor(Anchor inAnvhor) { m_anchor = inAnvhor; }
	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetSize(const D3DXVECTOR2& inSize);
	void SetColor(const D3DXCOLOR& inColor);
	void SetColorAlpha(const float inAlpha);
	void SetTexture(std::string inTex) { m_texture = inTex; }	// テクスチャの設定

	// Getter
	const D3DXVECTOR2 GetSize() { return m_size; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// VtxBuff
	Anchor m_anchor;	// アンカーの設定
	std::string m_texture;	// テクスチャ
	float m_rotY;			// 回転
	float m_fLength;		// 長さ
	float m_fAngle;		// 角度


protected:
	D3DXVECTOR2 m_size;	// 大きさ
	D3DXCOLOR m_col;		// 色
};

#endif // !_OBJECT2D_H_
