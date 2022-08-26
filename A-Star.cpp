//=============================================================================
//
// 参考書　ゲームプログラミングC++、ゲーム開発のためのAI入門,後学校に会った奴名前後で書き換えとく
// Author 磯江寿希亜
//
//=============================================================================
// 概要：
/*
	幅優先探索は始点から均等に探索するから効率が悪い
	最良優先探索はゴールに近い頂点を最良としている(今回は)
	A*は 最良優先探索の改良版　頂点までの距離　+　頂点までの距離	で計算　
	なぜA*なのか？	Q&A 頂点までの距離　+　頂点までの距離なのかそれは [袋小路] から脱出しやすくするため
	頑張って説明してみたけど分からなかったら調べてみてくれ
*/


//ディファインデリート
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"

//Thing[1]			ブロック
//Thing[2]			追いかける側
//Thing[3]			追いかけられる側

//-----------------------------------------------------------------------------
// コンストラクタ
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
ASTAR::ASTAR()
{	
	//初期化
	ZeroMemory(this,sizeof(ASTAR));

	//座標の周り8方向　周囲8セルに移動する際に利用
	m_ptVolute[0].x=0;	//移動なし 
	m_ptVolute[0].y=0;

	m_ptVolute[1].x=-1;	
	m_ptVolute[1].y=1;

	m_ptVolute[2].x=0;
	m_ptVolute[2].y=1;

	m_ptVolute[3].x=1;
	m_ptVolute[3].y=1;

	m_ptVolute[4].x=1;
	m_ptVolute[4].y=0;

	m_ptVolute[5].x=1;
	m_ptVolute[5].y=-1;

	m_ptVolute[6].x=0;
	m_ptVolute[6].y=-1;

	m_ptVolute[7].x=-1;
	m_ptVolute[7].y=-1;

	m_ptVolute[8].x=-1;
	m_ptVolute[8].y=0;
	
	/* 番号と座標の対応
	1 2 3
    8 0 4
    7 6 5
	*/	
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
ASTAR::~ASTAR()
{
	SAFE_DELETE_ARRAY(m_pOpenList);
	SAFE_DELETE_ARRAY(m_pClosedList);
	SAFE_DELETE_ARRAY(m_ptPath);
}

//-----------------------------------------------------------------------------
// 初期化
// Author 磯江寿希亜
// 説明：最終的なパスを格納するバッファのメモリ確保
//-----------------------------------------------------------------------------
HRESULT ASTAR::Init(ASTAR_INIT* pai)
{
	//
	m_dwCellWidth = pai->dwCellWidth;
	m_dwCellHeight = pai->dwCellHeight;
	m_pCell = pai->pCell;

	//動的確保
	m_pOpenList = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
	if(!m_pOpenList)
	{//メモリ不足のエラー
		return E_OUTOFMEMORY;
	}

	m_pClosedList = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
	if(!m_pClosedList)
	{//メモリ不足のエラー
		return E_OUTOFMEMORY;
	}

	m_ptPath = (D3DXVECTOR2*) new D3DXVECTOR2[m_dwCellWidth * m_dwCellHeight];
	if(!m_ptPath)
	{//メモリ不足のエラー
		return E_OUTOFMEMORY;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// リセット
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
HRESULT ASTAR::Reset()
{
	DWORD dwMaxAmt = m_dwCellWidth * m_dwCellHeight;

	for (int i = 0; i < dwMaxAmt; i++)
	{
		m_pCell[i].iCost = 0;
		m_pCell[i].iHeuristic = 0;
		m_pCell[i].iScore = 0;
		m_pCell[i].boRealPath = FALSE;
		ZeroMemory(&m_pCell[i].ptParent, sizeof(D3DXVECTOR2));
		if (m_pCell[i].Status != ASTAR_OBSTACLE)
		{
			m_pCell[i].Status = ASTAR_EMPTY;
		}
	}

	m_dwOpenAmt = 0;
	m_dwClosedAmt = 0;
	m_dwPathList = 0;
	ZeroMemory(m_ptPath, sizeof(D3DXVECTOR2) * m_dwCellWidth * m_dwCellHeight);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Author 磯江寿希亜
// 説明：HRESULT型は成功したかどうかを返している
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	int idx = (int)pParam->ptGoalPos.y * m_dwCellWidth + (int)pParam->ptGoalPos.x;	// ゴールに指定されたセルの番号

	//ゴールが適正な場所か（ゴールが障害物の内部になっていないか）
	if (m_pCell[idx].Status == ASTAR_OBSTACLE)
	{
		MessageBox(0, "obstacle", "", MB_OK);	// MB_OKはテキストボックスでokボタンを出させるための引数
		return E_FAIL;	// 失敗したならS_OK出なくE_FAILを返す事になりエラーを吐く
	}

	/* ↓ゴールが適切だった場合↓ */

	Reset();	// リセット

	// A*での調査
	if (SUCCEEDED(CalcScore(pParam)))
	{
		MakePathFromClosedList(pParam);
	}
	else
	{
		MessageBox(0, "動けない", "", MB_OK);
	}
	return S_OK;	// 成功したときにS_OKを返す
}

//-----------------------------------------------------------------------------
// １回の呼び出しで8方向セルの調査を行う
// Author 磯江寿希亜
// 説明：再帰しながらゴールまでの計算をすべて行う
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcScore(ASTAR_PARAM* pParam)
{
	int iOffset;

	D3DXVECTOR2 ptStart = pParam->ptStartPos;	// スタート位置
	D3DXVECTOR2 ptGoal = pParam->ptGoalPos;		// ゴール位置
	D3DXVECTOR2 ptCurrentPos = pParam->ptCurrentPos;	// 現在地

	// 現在地のセルは無条件でクローズドにする
	iOffset = ptCurrentPos.x * m_dwCellHeight + ptCurrentPos.y;

	if (m_pCell[iOffset].Status != ASTAR_CLOSED)
	{
		m_pCell[iOffset].Status = ASTAR_CLOSED;
		m_dwClosedAmt++;
	}

	// 隣接8セルのスコアリング　進めるマスを検索中
	for (int i = 1; i <= 8; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		//セルマップの範囲外に到達した場合
		if ((ptCurrentPos.x < 0) || ((DWORD)ptCurrentPos.x > m_dwCellWidth) || (ptCurrentPos.y < 0) || ((DWORD)ptCurrentPos.y > m_dwCellHeight))
		{
			continue;	//処理をスキップする
		}

		/* ↓現在地がセルマップの範囲内だった場合↓ */

		iOffset = ptCurrentPos.x * m_dwCellHeight + ptCurrentPos.y;

		//ゴールセルに到達した場合
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			m_pCell[iOffset].ptParent = pParam->ptCurrentPos;//親セルへのリンク
			m_pCell[iOffset].Status = ASTAR_GOAL;
			m_ptGoal = m_pCell[iOffset].ptIndex;
			return S_OK;
		}

		/* ↓まだゴールではない場合↓ */

		switch (m_pCell[iOffset].Status)
		{
		case ASTAR_EMPTY:	//進むとき
			m_pCell[iOffset].iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			//コストは親のコストに１を足したもの
			{
				m_pCell[iOffset].iCost = m_pCell[(int)pParam->ptCurrentPos.x * m_dwCellHeight + (int)pParam->ptCurrentPos.y].iCost + 1;
			}
			m_pCell[iOffset].iScore = m_pCell[iOffset].iCost + m_pCell[iOffset].iHeuristic;
			m_pCell[iOffset].Status = ASTAR_OPEN;
			m_pOpenList[m_dwOpenAmt] = ptCurrentPos;
			m_dwOpenAmt++;
			//８セルの親を現在セルにする
			{
				m_pCell[iOffset].ptParent = pParam->ptCurrentPos;//
			}
			break;
		case ASTAR_OPEN:
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			continue;
			break;
		}
	}

	//オープンリストを含めて最小スコアを、新たな起点セルにする　（クローズドリストの作成）
	int iMinScore = INT_MAX;
	D3DXVECTOR2 ptNextCellIndex = { 0 ,0 };
	DWORD dwMinIndex = 0;

	for (int i = 0; i < m_dwOpenAmt; i++)
	{
		iOffset = m_pOpenList[i].x * m_dwCellHeight + m_pOpenList[i].y;

		if (m_pCell[iOffset].iScore < iMinScore && m_pCell[iOffset].Status == ASTAR_OPEN)
		{
			iMinScore = m_pCell[iOffset].iScore;
			dwMinIndex = i;
		}
	}

	iOffset = m_pOpenList[dwMinIndex].x * m_dwCellHeight + m_pOpenList[dwMinIndex].y;
	ptNextCellIndex = m_pCell[iOffset].ptIndex;

	m_pClosedList[m_dwClosedAmt] = ptNextCellIndex;

	//新たな起点セルによる再帰呼び出し
	ASTAR_PARAM Param;
	Param.ptStartPos = pParam->ptStartPos;
	Param.ptGoalPos = pParam->ptGoalPos;
	Param.ptCurrentPos = m_pOpenList[dwMinIndex];
	Param.pCell = pParam->pCell;

	return CalcScore(&Param);
}

//-----------------------------------------------------------------------------
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
HRESULT ASTAR::MakePathFromClosedList(ASTAR_PARAM* pParam)
{
	int iBreak = 0;
	int iMaxStep = m_dwCellWidth * m_dwCellHeight;	//総合のマス
	m_dwPathList = 0;
	// クローズドセル上で、ゴール地点からスタート地点まで親を辿った経路を最終的なパスとする

	int iOffset = m_ptGoal.x*m_dwCellHeight + m_ptGoal.y;	// 番号のID

	//whileと違い１回は必ず通る
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "親のリンクがスタート地点まで繋がっていないか、リンクが循環しています", "", MB_OK);
			break;
		}
		iBreak++;
		m_pCell[iOffset].boRealPath = TRUE;
		iOffset = m_pCell[iOffset].ptParent.x*m_dwCellHeight + m_pCell[iOffset].ptParent.y;
		//パスリストに記録（呼び出し元の利便性)	クリアリング
		m_ptPath[m_dwPathList] = m_pCell[iOffset].ptIndex;
		m_dwPathList++;
	} while (m_pCell[iOffset].ptIndex.x != pParam->ptStartPos.x || m_pCell[iOffset].ptIndex.y != pParam->ptStartPos.y);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 目的と最終地の入力
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
int ASTAR::CalcDistance(D3DXVECTOR2* pptA, D3DXVECTOR2* pptB)
{
	int iX = pptA->x - pptB->x;
	int iY = pptA->y - pptB->y;
	return (int)sqrtf((float)iX*iX + (float)iY*iY);
}