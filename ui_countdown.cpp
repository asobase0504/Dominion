#include "application.h"
#include "ui_countdown.h"


const int CCountDownUI::READY_TIME = 60;		// REDEY終了までの時間
const int CCountDownUI::GO_TIME = 30;			// GO終了までの時間
const int CCountDownUI::PRIORITY_BELONG = 5;

CCountDownUI * CCountDownUI::Create(const D3DXVECTOR2 & inPos)
{
	CCountDownUI* countDown = new CCountDownUI;

	if (FAILED(countDown->Init()))
	{
		return nullptr;
	}

	// フレームの初期化
	{
		countDown->m_fream = new CObject2D(CObject::TYPE::NONE, 5);
		if (FAILED(countDown->m_fream->Init()))
		{
			return nullptr;
		}
		countDown->m_fream->SetPos((D3DXVECTOR3)inPos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(0);
		color.a = 0.95f;
		countDown->m_fream->SetColor(color);
		countDown->m_fream->SetSize({ (float)CApplication::GetInstance()->SCREEN_WIDTH,100.0f });
		countDown->m_fream->SetShouldStopAlsoUpdate();
	}

	// テキストの初期化
	{
		countDown->m_text = new CObject2D(CObject::TYPE::NONE, 5);
		if (FAILED(countDown->m_text->Init()))
		{
			return nullptr;
		}
		countDown->m_text->SetPos((D3DXVECTOR3)inPos);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor(1);
		color.a = 0.95f;
		countDown->m_text->SetColor(color);
		countDown->m_text->SetSize({ 300.0f,80.0f });
		countDown->m_text->SetTexture("TEXT_READY");
		countDown->m_text->SetShouldStopAlsoUpdate();
	}

	return countDown;
}

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CCountDownUI::CCountDownUI() :
	m_pos({0.0f,0.0f}),
	m_fream(nullptr),
	m_text(nullptr),
	m_count(0),
	m_isDeleted(false)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CCountDownUI::~CCountDownUI()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CCountDownUI::Init()
{
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CCountDownUI::Uninit()
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CCountDownUI::Update()
{
	CObject::SetStopUpdate(true);
	m_count++;

	if (m_count >= READY_TIME)
	{
		m_text->SetTexture("TEXT_GO");
	}
	if (m_count >= READY_TIME + GO_TIME)
	{
		CObject::SetStopUpdate(false);
		SetIsDeleted();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CCountDownUI::Draw()
{
}

//--------------------------------------------------
// オブジェクトを死亡させる
//--------------------------------------------------
void CCountDownUI::SetIsDeleted()
{
	m_isDeleted = true;
	m_text->SetIsDeleted(true);
	m_fream->SetIsDeleted(true);
}
