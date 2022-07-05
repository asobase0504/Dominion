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
class COperate;

//-----------------------------------------
// プレイヤークラス
//-----------------------------------------
class CCharacter : public CObject2D
{
public:
	CCharacter();
	~CCharacter();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Move();
	void BulletShot();

	// 命令者の設定
	void SetOperate(COperate* inOperate) { m_operate = inOperate; }

	static CCharacter* Create();
private:
	COperate* m_operate;	// 命令を出す人
};
#endif // !_CHARACTER_H_
