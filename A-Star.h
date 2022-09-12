//===========================================================
//
//参考書　ゲームプログラミングC++、ゲーム開発のためのAI入門,後学校に会った奴名前後で書き換えとく
//
//　Author 磯江寿希亜
//
//===========================================================
#ifndef _ASTAR_H_
#define _ASTAR_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <d3dx9.h>
#include <map>
#include <vector>
#include "character.h"

// 前方宣言
class CBlock;

//データ型定義
enum ASTAR_STATUS
{
	ASTAR_EMPTY,	// 未探索
	ASTAR_OPEN,		// 最短ルート候補
	ASTAR_CLOSED,	// 最短ルート
	ASTAR_OBSTACLE,	// 障害物
	ASTAR_GOAL,		// ゴール
};

struct CELL
{
	POINT ptIndex;	// 番号
	POINT ptParent;	// 親	
	int iCost;				// 実コスト
	int iHeuristic;			// 推定コスト
	int iScore;				// 合計スコア
	ASTAR_STATUS Status;	// ステータスを保持する変数
	bool boRealPath;		// 調査が完了したときに経路上にあるセルにおいて、このフラグがTRUEになる。つまりこのフラグが１の時セルを並べると経路になる
};

struct ASTAR_INIT
{
	CELL* pCell;
	int dwCellWidth;
	int dwCellHeight;
};

struct ASTAR_PARAM	// マップ情報的な？
{
	CELL* pCell;	// セル
	POINT ptStartPos;	// スタートの位置
	POINT ptGoalPos;	// ゴールの位置
	POINT ptCurrentPos;		//現在地
};

class ASTAR
{
public:
	//コンストラクタ
	ASTAR();
	//デストラクタ
	~ASTAR();

	HRESULT Init(ASTAR_INIT*);
	HRESULT CalcPath(ASTAR_PARAM*);	//パス計算関数

	// Getter
	POINT* GetPath() { return m_ptPath; }
	int GetPathList() { return m_dwPathList; }

	// Setter
	void SetCellStage(std::vector<std::vector<CBlock*>>& , CCharacter::TEAM);

protected: // 引継ぎ可能な関数
	HRESULT CalcScore(ASTAR_PARAM*);	// 最重要関数これが消えたら終わり	（スコアの計算、ゴールまでのクロースドリストを作る）
	HRESULT MakePathFromClosedList(ASTAR_PARAM*);	// 最終パスの制作
	int CalcDistance(POINT*, POINT*);	// プレイヤーと敵の距離を求めているだけ
	HRESULT Reset();	// 再検索が可能になるため

private:
	POINT* m_ptPath;	// 最終的に検索した最短経路セル群へのpointer
	int m_dwPathList;		// 最終パスを形成するセルの個数

	// ブロック情報
	std::vector<std::vector<CBlock*>>* m_stage;	// ステージ情報
	std::map<CBlock**, CELL> m_cell;	// 全てのセル情報
	CBlock* m_startBlock;		// スタートブロック
	CBlock* m_goalBlock;		// ゴールブロック

	CELL* m_pCell;				// すべてのセルの先頭セル
	POINT* m_pOpenList;	// オープンリストの先頭pointer
	int m_dwOpenAmt;			// オープンリスト内のセルの数
	POINT* m_pClosedList;	// クロースリストの先頭pointer
	int m_dwClosedAmt;		// クロースリスト内のセルの数

	int m_widthSize;		// 幅のサイズ数
	int m_heightSize;		// 高さのサイズ数

	POINT m_ptVolute[9+1];	// 周囲8方向を参照するさいの利便性のため	[1]左上[2]上[3]右上[4]左　	ここから先は多分　[5]左下[6]下[7]右下[8]右
	POINT m_ptGoal;			// ゴールの変数
};
#endif