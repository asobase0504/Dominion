//==================================================
// 
// 当たり判定のヘッダー
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
// 矩形の線分同士
//--------------------------------------------------
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// 矩形と矩形
//--------------------------------------------------
bool RectangleAndRectangle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2);

//--------------------------------------------------
// 矩形上側の線分と矩形
//--------------------------------------------------
bool RectangleTop(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// 矩形下側の線分と矩形
//--------------------------------------------------
bool RectangleDown(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// 矩形左側の線分と矩形
//--------------------------------------------------
bool RectangleLeft(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// 矩形右側の線分と矩形
//--------------------------------------------------
bool RectangleRight(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

//--------------------------------------------------
// 点と矩形
//--------------------------------------------------
bool PointAndRectangle(const D3DXVECTOR3& pointPos, const D3DXVECTOR3& rectanglePos, const D3DXVECTOR3& rectangleSize);

//--------------------------------------------------
// 円同士
//--------------------------------------------------
bool CircleAndCircle(const D3DXVECTOR3& pos1, const float size1, const D3DXVECTOR3& pos2, const float size2);

//--------------------------------------------------
// 線分同士
//--------------------------------------------------
float Vec2Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
}
#endif // !_COLLISION_H_
