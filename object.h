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
#include <vector>

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

	static void SetStopUpdate(bool inStopStatus) { isStopUpdate = inStopStatus; }

	static const std::list<CObject*>* GetMyObject(int inIndex) { return &object.at(inIndex); }
	static int GetPrioritySize() { return (int)object.size(); }
public:
	CObject(TYPE type,int priority = 3);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// Setter
	void SetIsDeleted(bool inDeleted) { m_isDeleted = inDeleted; }		// ���S��Ԃ�ݒ�
	void SetShouldStopAlsoUpdate() { shouldStopAlsoUpdate = true; }
	virtual void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }	// �ʒu�̐ݒ�

	// Getter
	const D3DXVECTOR3& GetPos() { return m_pos; }	// �ʒu�̎擾
	const TYPE GetType() { return m_type; }
	const bool GetIsDeleted() { return m_isDeleted; }

private:
	auto Release();

private:
	static std::vector<std::list<CObject*>> object;	// �I�u�W�F�N�g
	static int numAll;	// �ő吔
	int createIdx;		// �����ԍ�
	static bool isStopUpdate;	// �X�V����~����
	bool shouldStopAlsoUpdate;	// ��~�����X�V���ׂ���
	bool shouldInvisible;		// �`����~�����ׂ���
protected:
	D3DXVECTOR3 m_pos;	// �ʒu
	bool m_isDeleted;	// �폜�t���O
	TYPE m_type;		// ���
};

#endif // !_RENDERER_H_
