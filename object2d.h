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

class CObject2D : public CObject
{
public:
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
	static CObject2D* Create();

public:
	CObject2D(TYPE type = TYPE::NONE);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetSize(const D3DXVECTOR2& inSize);
	void SetCol(const D3DXCOLOR& inColor);
	void SetTexture(CTexture::TEXTURE inTex) { m_texture = inTex; }

	const D3DXVECTOR2 GetSize() { return size; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// VtxBuff
	CTexture::TEXTURE m_texture;	// �e�N�X�`��
	float rotY;			// ��]
	float fLength;		// ����
	float fAngle;		// �p�x

protected:
	D3DXVECTOR2 size;	// �傫��
	D3DXCOLOR col;		// �F
};

#endif // !_OBJECT2D_H_
