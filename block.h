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
#include <vector>

class CBlockScraped;
class CBlockColorAddition;

//--------------------------------------------------
// ブロッククラス
//--------------------------------------------------
class CBlock : public CObject2D
{
private:
	static const int PRIORITY_BELONG;
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
	//--------------------------------------------------
	// ブロック状態の列挙型
	//--------------------------------------------------
	enum DIRECTION
	{
		TOP = 0,
		DOWN,
		LEFT,
		RIGHT,
	};

public:
	CBlock(CObject::TYPE type = CObject::TYPE::BLOCK);
	~CBlock() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBlock* Create(BLOCK_TYPE type);
	void ChangeType(DIRECTION inDirection);
	BLOCK_TYPE GetType();

	void ResurveyRidingObject();

	void DeleteColorAddition() { m_colorAddition = nullptr; }

	// Setter
	void SetRidingObject(CObject* inObject);
	void SetAdditionColor();

	// Getter
	std::vector<CObject*> GetRidingObject() { return ridingObject; }

private:
	BLOCK_TYPE m_team;	// 状態
	CBlockScraped* m_scraped;
	CBlockColorAddition* m_colorAddition;
	std::vector<CObject*> ridingObject;
};
#endif // !_BLOCK_H_
