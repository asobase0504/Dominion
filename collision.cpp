#include "collision.h"

namespace Collision
{
bool Segment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* OutPos, float* t1, float* t2);

bool RectangleTop(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += -size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}

	}

	return false;
}

bool RectangleDown(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}

	}

	return false;
}

bool RectangleLeft(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += -size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

bool RectangleRight(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (Segment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//
// 点と矩形の当たり判定
//
bool PointAndRectangle(const D3DXVECTOR3 & pointPos, const D3DXVECTOR3 & rectanglePos, const D3DXVECTOR3 & rectangleSize)
{
	if (rectanglePos.x - rectangleSize.x > pointPos.x)
	{
		return false;
	}
	if (rectanglePos.x + rectangleSize.x < pointPos.x)
	{
		return false;
	}
	if (rectanglePos.y - rectangleSize.y > pointPos.y)
	{
		return false;
	}
	if (rectanglePos.y + rectangleSize.y < pointPos.y)
	{
		return false;
	}

	return true;
}

//
// 円と円の当たり判定
//
bool CircleAndCircle(const D3DXVECTOR3& pos1, const float size1, const D3DXVECTOR3& pos2, const float size2)
{
	D3DXVECTOR3 dist = pos1 - pos2;
	float c = sqrtf(dist.x * dist.x + dist.y * dist.y);

	if (c <= size1 + size2)
	{
		return true;
	}

	return false;
}

//
// 線分と線分の当たり判定
//
bool Segment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	// ベクトルの始点同士の距離。
	D3DXVECTOR3 v = start2 - start1;

	// ブロックのベクトルと被対象のベクトルが平行か調べる
	float Bv_Tv = Vec2Cross(vec1, vec2);
	if (Bv_Tv == 0.0f)
	{
		// 並行である。
		return false;
	}

	float v_Bv = Vec2Cross(v, vec1);
	float v_Tv = Vec2Cross(v, vec2);

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
	{
		return false;
	}

	if (t1 != NULL)
	{
		*t1 = hit1;
	}

	if (t2 != NULL)
	{
		*t2 = hit2;
	}

	if (outPos != NULL)
	{
		*outPos = start1 + vec1 * hit1;
	}
	return true;

}

//====================================
// 線分の外積処理
//====================================
float Vec2Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

}
