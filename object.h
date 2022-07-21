//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <d3dx9.h>
#include <list>

class CObject
{
public:	// �񋓌^
	// CObject�N���X�̔h����
	enum class TYPE
	{
		CHARACTER = 0,
		BULLET,
		BLOCK,
		NONE
	};

public:	// �萔
	static const int NUM_MAX = 1500;

public:	// �ÓI�֐�
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();
	static const std::list<CObject*>* GetMyObject() { return &object; }

public:
	CObject(TYPE type);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �O���A�N�Z�X�p
	const D3DXVECTOR3& GetPos() { return m_pos; }	// �ʒu�̎擾
	virtual void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }	// �ʒu�̐ݒ�
	const TYPE GetType() { return m_type; }
	const bool GetIsDeleted() { return m_isDeleted; }

private:
	auto Release();

private:
	static std::list<CObject*> object;	// �I�u�W�F�N�g
	static int numAll;	// �ő吔
	int createIdx;		// �����ԍ�

protected:
	D3DXVECTOR3 m_pos;	// �ʒu
	bool m_isDeleted;	// �폜�t���O
	TYPE m_type;		// ���
};

#endif // !_RENDERER_H_
