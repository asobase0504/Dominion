//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "object.h"
#include "texture.h"
#include <string>

class CObject2D : public CObject
{
public:

	enum Anchor
	{
		CENTER = 0,
		TOP,
		DOWN,
		LEFT,
		RIGHT,
		TOPLEFT,
		TOPRIGHT,
		DOWNLEFT,
		DOWNRIGHT,
	};

	//*****************************************************************************
	// �\���̒�`
	//*****************************************************************************
	// ���_�f�[�^
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

public:	// �ÓI�֐�
	static CObject2D* Create(TYPE type = TYPE::NONE, int inPriority = 3);

public:
	CObject2D(TYPE type = TYPE::NONE, int inPriority = 3);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetAnchor(Anchor inAnvhor) { m_anchor = inAnvhor; }
	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetSize(const D3DXVECTOR2& inSize);
	void SetColor(const D3DXCOLOR& inColor);
	void SetColorAlpha(const float inAlpha);
	void SetTexture(std::string inTex) { m_texture = inTex; }	// �e�N�X�`���̐ݒ�

	// Getter
	const D3DXVECTOR2 GetSize() { return m_size; }
	const D3DXCOLOR GetColor() { return m_col; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// VtxBuff
	Anchor m_anchor;	// �A���J�[�̐ݒ�
	std::string m_texture;	// �e�N�X�`��
	float m_rotY;			// ��]
	float m_fLength;		// ����
	float m_fAngle;		// �p�x


protected:
	D3DXVECTOR2 m_size;	// �傫��
	D3DXCOLOR m_col;		// �F
};

#endif // !_OBJECT2D_H_
