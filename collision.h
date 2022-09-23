//==================================================
// 
// �����蔻��̃w�b�_�[
// Author : Yuda Kaito
// 
//==================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//--------------------------------------------------
// include
//--------------------------------------------------
#include <d3dx9.h>

namespace Collision
{
enum class RECTANGLE_DIRECTION
{
	TOP = 0,
	DOWN,
	LEFT,
	RIGHT
};

//--------------------------------------------------
// ��`�̐������m
//--------------------------------------------------
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// ��`�Ƌ�`
//--------------------------------------------------
bool RectangleAndRectangle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2);

//--------------------------------------------------
// ��`�㑤�̐����Ƌ�`
//--------------------------------------------------
bool RectangleTop(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// ��`�����̐����Ƌ�`
//--------------------------------------------------
bool RectangleDown(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// ��`�����̐����Ƌ�`
//--------------------------------------------------
bool RectangleLeft(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// ��`�E���̐����Ƌ�`
//--------------------------------------------------
bool RectangleRight(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// �_�Ƌ�`
//--------------------------------------------------
bool PointAndRectangle(const D3DXVECTOR3& pointPos, const D3DXVECTOR3& rectanglePos, const D3DXVECTOR3& rectangleSize);

//--------------------------------------------------
// �~���m
//--------------------------------------------------
bool CircleAndCircle(const D3DXVECTOR3& pos1, const float size1, const D3DXVECTOR3& pos2, const float size2);

//--------------------------------------------------
// �������m
//--------------------------------------------------
float Vec2Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
}
#endif // !_COLLISION_H_
