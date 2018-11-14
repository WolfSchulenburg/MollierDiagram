#pragma once
#include "stdafx.h"

#include <limits>
#include <vector>

// Use the C header for _isnan() function
// because Microsoft doesn't supply the C++ std::isnan() function 
#include <float.h>

class CWorldPoint
{
public:
	double x;
	double y;

	bool hasValue() const
	{
		return ! (_isnan(x) || _isnan(y));
	}

	void ForceLargerSize() {
		if (x > y) y = x;
		else x = y;
	}

	void ForceMinSize(double size) {
		if (x < size) x = size;
		if (y < size) y = size;
	}

	CWorldPoint()
	{
		// Initialize with not-a-number
		x = std::numeric_limits<double>::quiet_NaN();
		y = std::numeric_limits<double>::quiet_NaN();
	}

	CWorldPoint(double ix, double iy)
	{
		x = ix;
		y = iy;
	}

	CWorldPoint operator+(CWorldPoint a)
	{
		CWorldPoint r(*this);
		r.x += a.x;
		r.y += a.y;
		return r;
	}

	CWorldPoint& operator+=(CWorldPoint a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	CWorldPoint& operator-=(CWorldPoint a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	CWorldPoint operator+(CPoint a)
	{
		CWorldPoint r(*this);
		r.x += a.x;
		r.y += a.y;
		return r;
	}
	CWorldPoint operator-(CPoint a)
	{
		CWorldPoint r(*this);
		r.x -= a.x;
		r.y -= a.y;
		return r;
	}
	CWorldPoint operator-(CWorldPoint a)
	{
		CWorldPoint r(*this);
		r.x -= a.x;
		r.y -= a.y;
		return r;
	}

	double Distance(CWorldPoint a) const
	{
		return sqrt( (a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
	}

	bool is_close_to(const CWorldPoint &a) const
	{
		const double close_distance = 0.001;
		return fabs(a.x - x) < close_distance && fabs(a.y - y) < close_distance;
	}

	bool operator==(const CWorldPoint &a) const
	{
		return is_close_to(a); // a.x == x && a.y == y;
	}

	bool operator!=(const CWorldPoint &a) const
	{
		return ! (*this == a);
	}

	bool operator<(const CWorldPoint &a) const
	{
		const double close_distance = 0.001;
		if (fabs(a.x - x) < close_distance)
		{
			return y < a.y - close_distance;
		}
		else
		{
			return x < a.x - close_distance;
		}
	}
};

