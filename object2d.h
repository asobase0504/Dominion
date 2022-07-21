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

class CObject2D : public CObject
{
public:
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

	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetSize(const D3DXVECTOR2& inSize);
	void SetCol(const D3DXCOLOR& inColor);
	void SetTexture(CTexture::TEXTURE inTex) { m_texture = inTex; }

	const D3DXVECTOR2 GetSize() { return size; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// VtxBuff
	CTexture::TEXTURE m_texture;	// テクスチャ
	float rotY;			// 回転
	float fLength;		// 長さ
	float fAngle;		// 角度

protected:
	D3DXVECTOR2 size;	// 大きさ
	D3DXCOLOR col;		// 色
};

#endif // !_OBJECT2D_H_
