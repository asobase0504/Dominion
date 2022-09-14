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
#define SAFE_DELETE(p)       { if(p) { delete (p);		(p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);	(p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release();	(p) = nullptr; } }

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"
#include "block.h"

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
	m_widthSize = pai->dwCellWidth;
	m_heightSize = pai->dwCellHeight;
	m_pCell = pai->pCell;

	//動的確保
	m_pOpenList = (POINT*) new POINT[m_widthSize * m_heightSize];
	if(!m_pOpenList)
	{//メモリ不足のエラー
		return E_OUTOFMEMORY;
	}

	m_pClosedList = (POINT*) new POINT[m_widthSize * m_heightSize];
	if(!m_pClosedList)
	{//メモリ不足のエラー
		return E_OUTOFMEMORY;
	}

	m_ptPath = (POINT*) new POINT[m_widthSize * m_heightSize];
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
	int dwMaxAmt = m_widthSize * m_heightSize;

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
	ZeroMemory(m_ptPath, sizeof(D3DXVECTOR2) * m_widthSize * m_heightSize);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Author 磯江寿希亜
// 説明：HRESULT型は成功したかどうかを返している
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	int idx = (int)pParam->ptGoalPos.y * m_widthSize + (int)pParam->ptGoalPos.x;	// ゴールに指定されたセルの番号

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
// ステージ情報の設定
// Author 湯田 海都
//-----------------------------------------------------------------------------
void ASTAR::SetCellStage(std::vector<std::vector<CBlock*>>& inStage , CCharacter::TEAM inTeam)
{
	m_stage = &inStage;

	for (int i = 0; i < (int)m_stage->size(); i++)
	{
		for (int j = 0; j < (int)m_stage->at(i).size(); j++)
		{
			CELL cell;
			cell.ptIndex;			// 番号
			cell.ptParent;			// 親	
			cell.iCost = 0;			// 実コスト
			cell.iHeuristic = 0;	// 推定コスト
			cell.iScore = 0;		// 合計スコア

			// 同じチームか否か
			bool isSameTeam = (int)inTeam == (int)m_stage->at(i).at(j)->CBlock::GetType();

			cell.Status = isSameTeam ? ASTAR_EMPTY : ASTAR_OBSTACLE;	// ステータスを設定する

			cell.boRealPath = false;	// 経路の情報の初期化

			// セルの追加
			m_cell.insert(std::make_pair(&m_stage->at(i).at(j), cell));
		}
	}
}

//-----------------------------------------------------------------------------
// １回の呼び出しで8方向セルの調査を行う
// Author 磯江寿希亜
// 説明：再帰しながらゴールまでの計算をすべて行う
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcScore(ASTAR_PARAM* pParam)
{
	POINT ptStart = pParam->ptStartPos;			// スタート位置
	POINT ptGoal = pParam->ptGoalPos;			// ゴール位置
	POINT ptCurrentPos = pParam->ptCurrentPos;	// 現在地

	// 現在地のセルは無条件でクローズドにする
	CBlock* thisBlock = m_stage->at(ptCurrentPos.x).at(ptCurrentPos.y);
	CELL* cell = &m_cell[&thisBlock];	// セル情報

	if (cell->Status != ASTAR_CLOSED)
	{
		cell->Status = ASTAR_CLOSED;
		m_dwClosedAmt++;
	}

	int iOffset;

	// 隣接8セルのスコアリング　進めるマスを検索中
	for (int i = 1; i <= 8; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		//セルマップの範囲外に到達した場合
		if ((ptCurrentPos.x < 0) || (ptCurrentPos.x > m_widthSize) || (ptCurrentPos.y < 0) || (ptCurrentPos.y >m_heightSize))
		{
			continue;	//処理をスキップする
		}

		/* ↓現在地がセルマップの範囲内だった場合↓ */

		iOffset = ptCurrentPos.x * m_heightSize + ptCurrentPos.y;
		thisBlock = m_stage->at(ptCurrentPos.x).at(ptCurrentPos.y);

		//ゴールセルに到達した場合
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			cell->ptParent = pParam->ptCurrentPos;//親セルへのリンク
			cell->Status = ASTAR_GOAL;
			m_ptGoal = cell->ptIndex;
			return S_OK;
		}

		/* ↓まだゴールではない場合↓ */

		switch (cell->Status)
		{
		case ASTAR_EMPTY:	//進むとき
			cell->iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			//コストは親のコストに１を足したもの
			{
				cell->iCost = m_pCell[pParam->ptCurrentPos.x * m_heightSize + pParam->ptCurrentPos.y].iCost + 1;
			}
			cell->iScore = cell->iCost + cell->iHeuristic;
			cell->Status = ASTAR_OPEN;
			m_pOpenList[m_dwOpenAmt] = ptCurrentPos;
			m_dwOpenAmt++;
			//８セルの親を現在セルにする
			{
				cell->ptParent = pParam->ptCurrentPos;//
			}
			break;
		case ASTAR_OPEN:
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			continue;
			break;
		}
	}

	/* ↓新たなCELLからルート検索を行う↓ */

	//オープンリストを含めて最小スコアを、新たな起点セルにする　（クローズドリストの作成）
	int iMinScore = INT_MAX;
	POINT ptNextCellIndex = { 0 ,0 };
	DWORD dwMinIndex = 0;

	// スコアが最小のセルを探す
	for (int i = 0; i < m_dwOpenAmt; i++)
	{
		thisBlock = m_stage->at(m_pOpenList[i].x).at(m_pOpenList[i].y);

		iOffset = m_pOpenList[i].x * m_heightSize + m_pOpenList[i].y;

		if (m_pCell[iOffset].iScore < iMinScore && m_pCell[iOffset].Status == ASTAR_OPEN)
		{
			iMinScore = m_pCell[iOffset].iScore;
			dwMinIndex = i;
		}
	}

	thisBlock = m_stage->at(m_pOpenList[dwMinIndex].x).at(m_pOpenList[dwMinIndex].y);
	iOffset = m_pOpenList[dwMinIndex].x * m_heightSize + m_pOpenList[dwMinIndex].y;
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
	int iMaxStep = m_widthSize * m_heightSize;	//総合のマス
	m_dwPathList = 0;
	// クローズドセル上で、ゴール地点からスタート地点まで親を辿った経路を最終的なパスとする

	int iOffset = m_ptGoal.x*m_heightSize + m_ptGoal.y;	// 番号のID

	//whileと違い１回は必ず通る
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "親のリンクがスタート地点まで繋がっていないか、リンクが循環しています", "", MB_OK);
			break;
		}
		iBreak++;
		m_pCell[iOffset].boRealPath = true;

		iOffset = m_pCell[iOffset].ptParent.x * m_heightSize + m_pCell[iOffset].ptParent.y;

		//パスリストに記録（呼び出し元の利便性)	クリアリング
		m_ptPath[m_dwPathList] = m_pCell[iOffset].ptIndex;
		m_dwPathList++;
	} while (m_pCell[iOffset].ptIndex.x != pParam->ptStartPos.x || m_pCell[iOffset].ptIndex.y != pParam->ptStartPos.y);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 距離の算出
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
int ASTAR::CalcDistance(POINT* pptA, POINT* pptB)
{
	int iX = pptA->x - pptB->x;
	int iY = pptA->y - pptB->y;
	return (int)sqrtf((float)iX*iX + (float)iY*iY);
}