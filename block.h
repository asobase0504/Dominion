//==================================================
// 
// デバッグ機能(ヘッダーファイル)
// Author : Yuda Kaito
// 
//==================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include "object2d.h"

//--------------------------------------------------
// ブロッククラス
//--------------------------------------------------
class CBlock : public CObject2D
{
public:
	//--------------------------------------------------
	// ブロック状態の列挙型
	//--------------------------------------------------
	enum BLOCK_TYPE
	{
		NONE = -1,
		BLOCK_01 = 0,
		BLOCK_02,
		MAX
	};

	static const int MAX_BLOCK = 40 * 17;

public:
	CBlock(CObject::TYPE type = CObject::TYPE::BLOCK);
	~CBlock() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBlock* Create(BLOCK_TYPE type);
	void ChangeType();
	BLOCK_TYPE GetType();

private:
	BLOCK_TYPE m_team;	// 状態
};
#endif // !_BLOCK_H_
