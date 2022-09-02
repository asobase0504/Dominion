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

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void Select(SELECT_DIRECTION inDirection);

	// Setter
	void SetSelectIdx(int Y, int X);
	void SetSelectIdx(std::vector<int> inIdx);

	// Getter
	const std::vector<int>& GetSelectIdx() { return m_selectIdx; }

	static CMenu* Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea,CMenuFream* inFream, std::vector<std::vector<CMenuItem*>> inItem);
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	D3DXVECTOR2 m_pos;	// �ʒu
	D3DXVECTOR2 m_Area;	// �͈�
	CMenuFream* m_fream;	// �t���[��
	std::vector<std::vector<CMenuItem*>> m_item;	// ����
	std::vector<int> m_selectIdx;					// ���ݑI�𒆂̔ԍ�
	D3DXVECTOR2 m_fInterval;						// ���ڂ̊Ԋu
	D3DXVECTOR2 m_AroundWhitespace;					// ����̗]��
};

//-----------------------------------------
// �t���[���N���X
//-----------------------------------------
class CMenuFream : public CObject2D
{
public:	// �p�u���b�N�֐�
	CMenuFream(CObject::TYPE type = CObject::TYPE::NONE);
	~CMenuFream() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

//-----------------------------------------
// ���ڃN���X
//-----------------------------------------
class CMenuItem : public CObject2D
{
public:	// �p�u���b�N�֐�
	CMenuItem(CObject::TYPE type = CObject::TYPE::NONE);
	~CMenuItem() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SelectUpdate();
	void NomalUpdate();
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

#endif // !_CHARACTER_H_
