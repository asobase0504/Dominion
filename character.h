//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CController;
class CBullet;
class CBlock;
class CRemaubsBullet;

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CCharacter : public CObject2D
{
public: // 定義
	static const int LIMIT_BULLET_COUNT;	// 弾の最大数
	static const int RELOAD_TIME;			// リロード時間

public: // 列挙型
	enum TEAM
	{
		TEAM_00,
		TEAM_01
	};

public:
	CCharacter(CObject::TYPE type = CObject::TYPE::CHARACTER);
	~CCharacter() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// Setter
	void SetController(CController* inOperate);
	void SetTeam(const TEAM inTeam);
	bool SetBlockIndex(const int count,std::vector<int> inIndex);

	// Getter
	std::vector<std::vector<int>> GetOfBlock() { return m_ofBlockIndex; }
	TEAM GetTeam() { return m_team; }

	static CCharacter* Create(TEAM inTeam);

	bool HitWithBlock(CBlock* inBlock);

private: // プライベート関数
	void ScreenFromOutTime();

	void Collision();
	void HitWithBullet(CBullet* inBullet);

private:
	CController* m_controller;	// 命令を出す人
	TEAM m_team;				// 所属してるチーム
	D3DXVECTOR3 m_move;			// 移動量
	std::vector<CRemaubsBullet*> m_remainsBulletDisplay;	// 表示数
	int m_remainsBulletCount;								// 残りの弾数
	int m_reloadCount;										// リロードカウント
	std::vector<std::vector<int>> m_ofBlockIndex;			// 乗ってるブロックの番号
	std::vector<int> m_ofBlockIndexCenter;					// 中心が乗ってるブロックの番号
	int m_ofBlockCount;										// 乗ってるブロックの個数
};

#endif // !_CHARACTER_H_
