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

//データ型定義
enum ASTAR_STATUS
{
	ASTAR_EMPTY,	// 空
	ASTAR_OPEN,		// 未探索
	ASTAR_CLOSED,	// 探索済み
	ASTAR_OBSTACLE,	// 障害物
	ASTAR_GOAL,		// ゴール
};

//自分自身の座標
struct CELL
{
	D3DXVECTOR2 ptIndex;
	D3DXVECTOR2 ptParent;
	int iCost;
	int iHeuristic;
	int iScore;
	ASTAR_STATUS Status;	//ステータスを保持する変数
	BOOL boRealPath;		//調査が完了したときに経路上にあるセルにおいて、このフラグがTRUEになる。つまりこのフラグが１の時セルを並べると経路になる
	VOID* ppv1;				//メッシュ		main.cppで使用する
};

struct ASTAR_INIT
{
	CELL* pCell;
	DWORD dwCellWidth;
	DWORD dwCellHeight;		
};

struct ASTAR_PARAM	// マップ情報的な？
{
	CELL* pCell;	// セル
	D3DXVECTOR2 ptStartPos;	// スタートの位置
	D3DXVECTOR2 ptGoalPos;	// ゴールの位置
	D3DXVECTOR2 ptCurrentPos;		//現在地
};

class ASTAR
{
public:
	//コンストラクタ
	ASTAR();
	//デストラクタ
	~ASTAR();
	D3DXVECTOR2* m_ptPath;	//最終的に検索した最短経路セル群へのpointer
	DWORD m_dwPathList;		//最終パスを形成するセルの個数

	HRESULT Init(ASTAR_INIT*);
	HRESULT CalcPath(ASTAR_PARAM*);	//パス計算関数
protected:
	CELL* m_pCell;				//すべてのセルの先頭セル
	D3DXVECTOR2* m_pOpenList;	//オープンリストの先頭pointer
	DWORD m_dwOpenAmt;			//オープンリスト内のセルの数
	D3DXVECTOR2* m_pClosedList;	//クロースリストの先頭pointer
	int m_dwClosedAmt;		//クロースリスト内のセルの数

	DWORD m_dwCellWidth;		//セルを幅
	DWORD m_dwCellHeight;		//セルを高さ

	D3DXVECTOR2 m_ptVolute[9+1];	//周囲8方向を参照するさいの利便性のため	[1]左上[2]上[3]右上[4]左　	ここから先は多分　[5]左下[6]下[7]右下[8]右
	D3DXVECTOR2 m_ptGoal;			//ゴールの変数

	HRESULT CalcScore(ASTAR_PARAM*);	//最重要関数これが消えたら終わり	（スコアの計算、ゴールまでのクロースドリストを作る）
	HRESULT MakePathFromClosedList(ASTAR_PARAM*);	//最終パスの制作
	int CalcDistance(D3DXVECTOR2*, D3DXVECTOR2*);	//プレイヤーと敵の距離を求めているだけ
	HRESULT Reset();	//再検索が可能になるため
};
#endif