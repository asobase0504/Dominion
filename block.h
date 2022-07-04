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
#include "main.h"
#include "object2d.h"

//--------------------------------------------------
// �u���b�N�N���X
//--------------------------------------------------
class CBlock : public CObject2D
{
public:
	//--------------------------------------------------
	// �u���b�N��Ԃ̗񋓌^
	//--------------------------------------------------
	enum class TYPE
	{
		NONE = 0,
		BLOCK_01,
		BLOCK_02,
		MAX
	};

	static const int MAX_BLOCK = 40 * 17;
public:
	CBlock();
	~CBlock();

	HRESULT Init() override;
	void Uninit();
	void Update();
	void Draw();

	static CBlock* Create(TYPE type);
	void ChangeType(TYPE type);
	TYPE GetType();

private:
	TYPE m_type;	// ���
};
#endif // !_BLOCK_H_
