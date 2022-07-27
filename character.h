//=========================================
// 
// プレイヤークラス
// Author YudaKaito
// 
//=========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object2d.h"

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CController;
class CBullet;
class CBlock;

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CCharacter : public CObject2D
{
public: // 定義
	static const int LIMIT_BULLET_COUNT;
	static const int RELOAD_TIME;

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

	// 命令者の設定
	void SetController(CController* inOperate) { m_controller = inOperate; }
	void SetTeam(const TEAM inTeam);

	static CCharacter* Create(TEAM inTeam);

private: // プライベート関数
	void ScreenFromOutTime();

	void Collision();
	void HitWithBlock(CBlock* inBlock);
	void HitWithBullet(CBullet* inBullet);

private:
	CController* m_controller;	// 命令を出す人
	TEAM m_team;				// 所属してるチーム
	D3DXVECTOR3 m_move;			// 移動量
	int m_remainsBulletCount;	// 残りの弾数
	int m_reloadCount;			// リロードカウント
};

#endif // !_CHARACTER_H_
