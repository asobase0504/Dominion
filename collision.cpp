#include "collision.h"

namespace Collision
{
bool Segment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* OutPos, float* t1, float* t2);

bool RectangleSegment(const RECTANGLE_DIRECTION direction, const D3DXVECTOR3 & pos1, const D3DXVECTOR3 & size1, const D3DXVECTOR3 & pos2, const D3DXVECTOR3 & size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	switch (direction)
	{
	case RECTANGLE_DIRECTION::TOP:
		return RectangleTop(pos1, size1, pos2, size2, outPos,t1,t2);
		break;
	case RECTANGLE_DIRECTION::DOWN:
		return RectangleDown(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::LEFT:
		return RectangleLeft(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::RIGHT:
		return RectangleRight(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	default:
		break;
	}
	return false;
}

//
// ��`�Ƌ�`�̓����蔻��(�S��)
//
bool RectangleAndRectangle(const D3DXVECTOR3 & pos1, const D3DXVECTOR3 & size1, const D3DXVECTOR3 & pos2, const D3DXVECTOR3 & size2)
{
	/* ���g�̔���p */
	float fLeft = pos1.x - (size1.x * 0.5f);		//���[
	float fRight = pos1.x + (size1.x * 0.5f);		//�E�[
	float fTop = pos1.y + (size1.y * 0.5f);			//��[
	float fBottom = pos1.y - (size1.y * 0.5f);		//���[

	/* �Ώۂ̔���p */
	float fLeftTarget = pos2.x - (size2.x * 0.5f);	//���[
	float fRightTarget = pos2.x + (size2.x * 0.5f);	//�E�[
	float fTopTarget = pos2.y + (size2.y * 0.5f);	//��[
	float fBottomTarget = pos2.y - (size2.y * 0.5f);//���[

	if (fLeft < fRightTarget
		&& fRight > fLeftTarget
		&& fTop > fBottomTarget
		&& fBottom < fTopTarget)
	{//�u���g�v���u�Ώۂ͈͓̔��v�ɗ����ꍇ
		return true;	//�u���������v��Ԃ�
	}

	return false;	//�u�������Ă��Ȃ��v��Ԃ�
}

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
// �_�Ƌ�`�̓����蔻��
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
// �~�Ɖ~�̓����蔻��
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
// �����Ɛ����̓����蔻��
//
bool Segment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	// �x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = start2 - start1;

	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = Vec2Cross(vec1, vec2);
	if (Bv_Tv == 0.0f)
	{
		// ���s�ł���B
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
// �����̊O�Ϗ���
//====================================
float Vec2Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

}
