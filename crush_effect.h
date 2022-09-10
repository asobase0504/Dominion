//=========================================
// 
// �ӂ���G�t�F�N�g�̏����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _CRUSH_EFFECT_H_
#define _CRUSH_EFFECT_H_

#include "object2d.h"

class CCrushEffect : public CObject2D
{
public:
	CCrushEffect(CObject::TYPE type = CObject::TYPE::NONE);
	~CCrushEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCrushEffect* Create(const D3DXVECTOR3& inPos);

private: // �v���C�x�[�g�֐�
private:
	int m_life;			// �̗�
	D3DXVECTOR3 m_move;	// �ړ���
};
#endif // !_PLAYER_H_
