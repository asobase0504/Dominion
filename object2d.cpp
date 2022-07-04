//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#include "object2d.h"
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CObject2D::CObject2D() :
	m_pVtxBuff(nullptr),
	m_texture(CTexture::TEXTURE::TEXTURE_NONE),
	rotY(0.0f),
	fLength(0.0f),
	fAngle(0.0f)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject2D::Init()
{
	// 頂点バッファの生成
	CApplication::GetInstance()->GetRenderer()->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = SCREEN_WIDTH * 0.5f + sinf(rotY + fAngle + -D3DX_PI) * fLength;
	pVtx[0].pos.y = SCREEN_HEIGHT * 0.5f + cosf(rotY + fAngle + -D3DX_PI) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH * 0.5f + sinf(rotY + -fAngle + D3DX_PI) * fLength;
	pVtx[1].pos.y = SCREEN_HEIGHT * 0.5f + cosf(rotY + -fAngle + D3DX_PI) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = SCREEN_WIDTH * 0.5f + sinf(rotY + fAngle * -1.0f) * fLength;
	pVtx[2].pos.y = SCREEN_HEIGHT * 0.5f + cosf(rotY + fAngle * -1.0f) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH * 0.5f + sinf(rotY + fAngle) * fLength;
	pVtx[3].pos.y = SCREEN_HEIGHT * 0.5f + cosf(rotY + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// texの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject2D::Uninit()
{
	// 頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObject2D::Update()
{
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 texture= CApplication::GetInstance()->GetTextureClass()->GetTexture(m_texture);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャの破棄
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 位置の設定
//=============================================================================
void CObject2D::SetPos(const D3DXVECTOR3 & inPos)
{
	CObject::SetPos(inPos);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x + sinf(rotY + fAngle + -D3DX_PI) * fLength;
	pVtx[0].pos.y = m_pos.y + cosf(rotY + fAngle + -D3DX_PI) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(rotY + -fAngle + D3DX_PI) * fLength;
	pVtx[1].pos.y = m_pos.y + cosf(rotY + -fAngle + D3DX_PI) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(rotY + fAngle * -1.0f) * fLength;
	pVtx[2].pos.y = m_pos.y + cosf(rotY + fAngle * -1.0f) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(rotY + fAngle) * fLength;
	pVtx[3].pos.y = m_pos.y + cosf(rotY + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 大きさの設定
//=============================================================================
void CObject2D::SetSize(const D3DXVECTOR2 & inSize)
{
	size = inSize;
	fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	SetPos(m_pos);
}

void CObject2D::SetCol(const D3DXCOLOR & inColor)
{
	col = inColor;

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = inColor;
	pVtx[1].col = inColor;
	pVtx[2].col = inColor;
	pVtx[3].col = inColor;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンクラスの作成
//=============================================================================
CObject2D* CObject2D::Create()
{
	if (numAll >= NUM_MAX)
	{
		return nullptr;
	}

	int idx = numAll;
	CObject2D* objectCreate = new CObject2D;
	objectCreate->createIdx = idx;

	if (objectCreate != nullptr)
	{
		if (FAILED(objectCreate->Init()))
		{
			assert(false);
		}
	}

	return objectCreate;
}
