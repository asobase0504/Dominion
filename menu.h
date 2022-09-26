//=========================================
// 
// ���j���[�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _MENU_H_
#define _MENU_H_

#include "object2d.h"
#include <vector>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CMenuFream;
class CMenuItem;

//-----------------------------------------
// ���j���[�N���X
//-----------------------------------------
class CMenu
{
public: // ��`
	static const int DECISION_AFTERGLOW_TIME;	// �����̗]�C����
public: // �񋓌^
	enum SELECT_DIRECTION
	{
		TOP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

public:	// �p�u���b�N�֐�
	CMenu();
	~CMenu();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Select(SELECT_DIRECTION inDirection);
	bool Decision(bool inDecison);
	virtual void UpdateBeforeDecision();
	virtual void UpdateAfterDecision();

	// �I�����ڂ̒ǉ�
	void AddItemX(CMenuItem* inItem , int indexY = 0);
	void AddItemY(CMenuItem* inItems,int indexX = 0);
	void AddItemY(std::vector<CMenuItem*> inItems);

	// Setter
	void SetSelectIdx(int Y, int X);
	void SetSelectIdx(std::vector<int> inIdx);
	void SetInterval(D3DXVECTOR2 inIntervar);
	void SetInterval(float inIntervarX, float inIntervarY);
	void SetIsDeleted();
	void SetItems(const std::vector<std::vector<CMenuItem*>>& inItems);

	// Getter
	const std::vector<int>& GetSelectIdx() { return m_selectIdx; }
	CMenuFream* GetFream() { return m_fream; }

	static CMenu* Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea,CMenuFream* inFream);
protected: // �v���C�x�[�g�֐�
	// Setter
	void SetItemPos();
protected: // �����o�[�ϐ�
	D3DXVECTOR2 m_pos;								// �ʒu
	D3DXVECTOR2 m_Area;								// �͈�
	CMenuFream* m_fream;							// �t���[��
	std::vector<std::vector<CMenuItem*>> m_item;	// ����
	std::vector<int> m_selectIdx;					// ���ݑI�𒆂̔ԍ�
	D3DXVECTOR2 m_fInterval;						// ���ڂ̊Ԋu
	D3DXVECTOR2 m_AroundWhitespace;					// ����̗]��
	bool m_isDexision;								// ���肵�Ă��邩�ۂ�
	int m_decisionAfterglowCount;					// �����̗]�C�J�E���g
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CMenuFream : public CObject2D
{
public:	// �p�u���b�N�֐�
	CMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CMenuFream() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ��{���ڃN���X
//-----------------------------------------
class CMenuItem : public CObject2D
{
public:	// �p�u���b�N�֐�
	CMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	virtual ~CMenuItem() override;

	virtual HRESULT Init() override;

	virtual void PopUpdate();
	virtual void SelectUpdate();
	virtual void NomalUpdate();
	virtual void EndUpdate();

	// Setter
	void SetPopDuringUpdateAccept(bool inAccept) { m_isPopDuringUpdateAccept = inAccept; }

	// Getter
	bool GetisPopNow() { return m_isPopNow; }

private: // �v���C�x�[�g�֐�

protected: // �����o�[�ϐ�
	bool m_isPopDuringUpdateAccept;	// �|�b�v�����̃A�b�v�f�[�g���󂯓���邩
	bool m_isPopNow;	// ���݃|�b�v����
};

#endif // !_CHARACTER_H_
