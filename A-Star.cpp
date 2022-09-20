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

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "A-Star.h"
#include "block.h"
#include <assert.h>

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, __FILE__, __LINE__, NULL, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

//-----------------------------------------------------------------------------
// コンストラクタ
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
ASTAR::ASTAR()
{	
	//初期化
	ZeroMemory(this,sizeof(ASTAR));

	//座標の周り8方向　周囲8セルに移動する際に利用
	m_ptVolute[0].x = 0;	//移動なし 
	m_ptVolute[0].y = 0;

	//m_ptVolute[1].x = -1;
	//m_ptVolute[1].y = 1;

	m_ptVolute[1].x = 0;
	m_ptVolute[1].y = 1;

	//m_ptVolute[3].x = 1;
	//m_ptVolute[3].y = 1;

	m_ptVolute[2].x = 1;
	m_ptVolute[2].y = 0;

	//m_ptVolute[5].x = 1;
	//m_ptVolute[5].y = -1;

	m_ptVolute[3].x = 0;
	m_ptVolute[3].y = -1;

	//m_ptVolute[7].x = -1;
	//m_ptVolute[7].y = -1;

	m_ptVolute[4].x = -1;
	m_ptVolute[4].y = 0;
	
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
	assert(m_openList.empty());
	assert(m_closedList.empty());
}

//-----------------------------------------------------------------------------
// 初期化
// Author 磯江寿希亜
// 説明：最終的なパスを格納するバッファのメモリ確保
//-----------------------------------------------------------------------------
HRESULT ASTAR::Init(std::vector<std::vector<CBlock*>>& inStage, CCharacter::TEAM inTeam)
{
	m_stage = &inStage;
	m_team = inTeam;

	// 大きさの取得
	m_widthSize = m_stage->at(0).size();
	m_heightSize = m_stage->size();

	m_openList.clear();
	m_closedList.clear();

	//Reset();
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author 湯田海都
//-----------------------------------------------------------------------------
void ASTAR::Uninit()
{
	m_path.clear();
	m_cell.clear();
	m_openList.clear();
	m_closedList.clear();
}

//-----------------------------------------------------------------------------
// リセット
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
HRESULT ASTAR::Reset()
{
	m_cell.clear();
	for (int y = 0; y < (int)m_stage->size(); y++)
	{
		for (int x = 0; x < (int)m_stage->at(y).size(); x++)
		{
			CELL cell;
			cell.ptIndex.x = x;		// 番号
			cell.ptIndex.y = y;		// 番号
			cell.ptParent.x = 0;	// 親	
			cell.ptParent.y = 0;	// 親	
			cell.iCost = 1;			// 実コスト

			int distX = m_startIndex.x - cell.ptIndex.x;
			int distY = m_startIndex.y - cell.ptIndex.y;

			if (distX >= distY)
			{
				cell.iHeuristic = distX;	// 推定コスト
			}
			else
			{
				cell.iHeuristic = distY;	// 推定コスト
			}

			cell.iScore = 0;		// 合計スコア

			// 同じチームか否か
			bool isSameTeam = (int)m_team == (int)m_stage->at(y).at(x)->CBlock::GetType();
			cell.Status = isSameTeam ? ASTAR_EMPTY : ASTAR_OBSTACLE;	// ステータスを設定する

			cell.boRealPath = false;	// 経路の情報の初期化

			// セルの追加
			m_cell.push_back(cell);
		}
	}

	m_openList.clear();
	m_closedList.clear();
	m_path.clear();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 番号からブロックを取得
// Author 湯田海都
//-----------------------------------------------------------------------------
int ASTAR::GetIndexToBlock(POINT inPoint)
{
	if (m_stage == nullptr)
	{
		assert(false);
	}

	if ((int)m_stage->size() <= inPoint.y)
	{
		return m_stage->at(m_stage->size() - 1).size() * m_stage->size() - 1 + inPoint.x;
	}

	return m_stage->at(inPoint.y).size() * inPoint.y + inPoint.x;
}

//-----------------------------------------------------------------------------
// 番号からブロックを取得
// Author 湯田海都
//-----------------------------------------------------------------------------
int ASTAR::GetIndexToBlock(int X, int Y)
{
	return 	m_stage->at(Y).size() * Y + X;
}

//-----------------------------------------------------------------------------
// Author 磯江寿希亜
// 説明：HRESULT型は成功したかどうかを返している
//-----------------------------------------------------------------------------
HRESULT ASTAR::CalcPath(ASTAR_PARAM* pParam)
{
	m_startIndex = pParam->ptStartPos;
	m_goalIndex = pParam->ptGoalPos;

	Reset();	// リセット

	// ゴールが適正な場所か（ゴールが障害物の内部になっていないか）
	if (m_cell[GetIndexToBlock(pParam->ptGoalPos)].Status == ASTAR_OBSTACLE)
	{
		DEBUG_PRINT("ゴールが障害物になってます\n");
		return E_FAIL;	// 失敗したならS_OK出なくE_FAILを返す事になりエラーを吐く
	}

	/* ↓ゴールが適切だった場合↓ */

	// A*での調査
	if (SUCCEEDED(CalcScore(pParam)))
	{
		MakePathFromClosedList(pParam);
	}
	else
	{
		DEBUG_PRINT("ルート構築が出来ません\n");
		return E_FAIL;
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
	POINT ptStart = pParam->ptStartPos;			// スタート位置
	POINT ptGoal = pParam->ptGoalPos;			// ゴール位置
	POINT ptCurrentPos = pParam->ptCurrentPos;	// 現在地
	int end = 0;

	// 現在地のセルは無条件でクローズドにする
	CELL* cell = &m_cell[GetIndexToBlock(ptCurrentPos)];	// セル情報

	if (cell->Status == ASTAR_CLOSED)
	{
		return E_FAIL;
	}

	if (cell->Status != ASTAR_CLOSED)
	{
		cell->Status = ASTAR_CLOSED;
		m_closedList.push_back(cell->ptIndex);
	}

	// 隣接8セルのスコアリング　進めるマスを検索中
	for (int i = 1; i <= 4; i++)
	{
		ptCurrentPos = pParam->ptCurrentPos;
		ptCurrentPos.x += m_ptVolute[i].x;
		ptCurrentPos.y += m_ptVolute[i].y;

		cell = &m_cell[GetIndexToBlock(ptCurrentPos)];

		// セルマップの範囲外に到達した場合
		if ((ptCurrentPos.x < 0) || (ptCurrentPos.x > m_widthSize) || (ptCurrentPos.y < 0) || (ptCurrentPos.y > m_heightSize))
		{
			continue;	// 処理をスキップする
		}

		/* ↓現在地がセルマップの範囲内だった場合↓ */

		// ゴールセルに到達した場合
		if (ptCurrentPos.x == ptGoal.x && ptCurrentPos.y == ptGoal.y)
		{
			cell->ptParent = pParam->ptCurrentPos;	// 親セルへのリンク
			cell->Status = ASTAR_GOAL;
			m_ptGoal = cell->ptIndex;
			return S_OK;
		}

		/* ↓まだゴールではない場合↓ */

		switch (cell->Status)
		{
		case ASTAR_EMPTY:	// 進むとき
			//cell->iHeuristic = CalcDistance(&ptGoal, &ptCurrentPos);
			// コストは親のコストに１を足したもの
			{
				cell->iCost = m_cell[GetIndexToBlock(pParam->ptCurrentPos)].iCost + 1;
			}
			cell->iScore = cell->iCost + cell->iHeuristic;
			cell->Status = ASTAR_OPEN;
			m_openList.push_back(ptCurrentPos);
			// ８セルの親を現在セルにする
			{
				cell->ptParent = pParam->ptCurrentPos;	// 
			}
			break;
		case ASTAR_OPEN:
			break;
		case ASTAR_CLOSED:
		case ASTAR_OBSTACLE:
			end++;
			continue;
			break;
		}
	}

	/* ↓新たなCELLからルート検索を行う↓ */

	// オープンリストを含めて最小スコアを、新たな起点セルにする　（クローズドリストの作成）
	int iMinScore = INT_MAX;
	DWORD dwMinIndex = 0;

	// スコアが最小のセルを探す
	for (int i = 0; i < (int)m_openList.size(); i++)
	{
		CELL minCell = m_cell[GetIndexToBlock(m_openList[i])];

		if (minCell.iScore < iMinScore && minCell.Status == ASTAR_OPEN)
		{
			iMinScore = minCell.iScore;
			dwMinIndex = i;
		}
	}

	//// クローズに入力
	//POINT ptNextCellIndex = { 0 ,0 };
	//ptNextCellIndex = m_cell[GetIndexToBlock(m_openList[dwMinIndex])].ptIndex;
	//m_closedList.push_back(ptNextCellIndex);

	// 新たな起点セルによる再帰呼び出し
	ASTAR_PARAM Param;
	Param.ptStartPos = pParam->ptStartPos;
	Param.ptGoalPos = pParam->ptGoalPos;
	Param.ptCurrentPos = m_openList[dwMinIndex];

	return CalcScore(&Param);
}

//-----------------------------------------------------------------------------
// Author 磯江寿希亜
//-----------------------------------------------------------------------------
HRESULT ASTAR::MakePathFromClosedList(ASTAR_PARAM* pParam)
{
	int iBreak = 0;
	int iMaxStep = m_widthSize * m_heightSize;	// 総合のマス

	// クローズドセル上で、ゴール地点からスタート地点まで親を辿った経路を最終的なパスとする

	POINT iOffset;	// 番号のID
	iOffset.x = m_ptGoal.x;
	iOffset.y = m_ptGoal.y;

	m_path.push_back(m_cell[GetIndexToBlock(iOffset)].ptIndex);

	// whileと違い１回は必ず通る
	do
	{
		if (iBreak > iMaxStep)
		{
			MessageBox(0, "親のリンクがスタート地点まで繋がっていないか、リンクが循環しています", "", MB_OK);
			return E_FAIL;
			break;
		}
		iBreak++;

		POINT parentPoint = m_cell[GetIndexToBlock(iOffset)].ptParent;
		m_cell[GetIndexToBlock(parentPoint)].boRealPath = true;
		
		// パスリストに記録（呼び出し元の利便性)	クリアリング
		m_path.push_back(m_cell[GetIndexToBlock(parentPoint)].ptIndex);

		iOffset = parentPoint;

	} while (m_cell[GetIndexToBlock(iOffset)].ptIndex.x != pParam->ptStartPos.x || m_cell[GetIndexToBlock(iOffset)].ptIndex.y != pParam->ptStartPos.y);

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
	return (int)sqrtf((float)iX * iX + (float)iY * iY);
}