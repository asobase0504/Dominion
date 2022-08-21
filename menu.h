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

public:	// �p�u���b�N�֐�
	CMenu();
	~CMenu();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CMenu* Create(D3DXVECTOR2 inPos, D3DXVECTOR2 inArea,CMenuFream* inFream, std::vector<std::vector<CMenuItem*>> inItem);
private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	D3DXVECTOR2 m_pos;	// �ʒu
	D3DXVECTOR2 m_Area;	// �ʒu
	CMenuFream* m_fream;	// �t���[��
	std::vector<std::vector<CMenuItem*>> m_item;	// ����
	int m_selectIdx;				// ���ݑI�𒆂̔ԍ�
	D3DXVECTOR2 m_fInterval;		// ���ڂ̊Ԋu
	D3DXVECTOR2 m_AroundWhitespace;	// ����̗]��
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

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

};

#endif // !_CHARACTER_H_
