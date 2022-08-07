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
// �u���b�N�N���X
//--------------------------------------------------
class CBlock : public CObject2D
{
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

	void ResurveyRidingObject();

	// Setter
	void SetRidingObject(CObject* inObject);

	// Getter
	std::vector<CObject*> GetRidingObject() { return ridingObject; }

private:
	BLOCK_TYPE m_team;	// ���
	std::vector<CObject*> ridingObject;
};
#endif // !_BLOCK_H_
