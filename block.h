//==================================================
// 
// �f�o�b�O�@�\(�w�b�_�[�t�@�C��)
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

//--------------------------------------------------
// �O���錾
//--------------------------------------------------
class CBlockScraped;
class CBlockColorAddition;
class CMap;

//--------------------------------------------------
// �u���b�N�N���X
//--------------------------------------------------
class CBlock : public CObject2D
{
private:
	static const int PRIORITY_BELONG;
public:
	//--------------------------------------------------
	// �u���b�N��Ԃ̗񋓌^
	//--------------------------------------------------
	enum BLOCK_TYPE
	{
		NONE = -1,
		BLOCK_01 = 0,
		BLOCK_02,
		MAX
	};
	//--------------------------------------------------
	// �u���b�N��Ԃ̗񋓌^
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

	static CBlock* Create(BLOCK_TYPE type, CMap* inParent);
	void ChangeType(DIRECTION inDirection);
	BLOCK_TYPE GetType();

	void ResurveyRidingObject();

	// ���
	void ColorAdditionDelete();
	void ScrapedDelete();

	// Setter
	void SetPosPlan(const D3DXVECTOR3& inPos);
	void SetSizePlan(const D3DXVECTOR2& inSize);
	void SetRidingObject(CObject* inObject);
	void SetAdditionColor();

	// Getter
	std::vector<CObject*> GetRidingObject() { return ridingObject; }

private: // �v���C�x�[�g�֐�
	void UpdateInPop();
private:
	BLOCK_TYPE m_team;	// ���
	CMap* m_parent;		// �e
	CBlockScraped* m_scraped;
	CBlockColorAddition* m_colorAddition;
	std::vector<CObject*> ridingObject;

	// �o���ɕK�v�ȃf�[�^
	bool m_isDuringPop;	// �o�������ۂ�
	D3DXVECTOR3 m_posPlan;	// �ŏI�\��ʒu
	D3DXVECTOR2 m_sizePlan;	// �ŏI�\��T�C�Y
};
#endif // !_BLOCK_H_
