#ifndef RECAST_SHARED_H
#define RECAST_SHARED_H

struct rcEdge
{
	unsigned short vert[2];
	unsigned short polyEdge[2];
	unsigned short poly[2];
};

// TODO: these are the same as in RecastMesh.cpp, consider using the same.
// Last time I checked the if version got compiled using cmov, which was a lot faster than module (with idiv).
inline int prevWrapped(int i, int n) { return i-1 >= 0 ? i-1 : n-1; }
inline int nextWrapped(int i, int n) { return i+1 < n ? i+1 : 0; }

template <class T>
inline bool uleft(const T* a, const T* b, const T* c)
{
	return ((int)b[0] - (int)a[0]) * ((int)c[2] - (int)a[2]) -
			((int)c[0] - (int)a[0]) * ((int)b[2] - (int)a[2]) < 0;
}

template <class T>
inline int area2(const T* a, const T* b, const T* c)
{
	return (b[0] - a[0]) * (c[2] - a[2]) - (c[0] - a[0]) * (b[2] - a[2]);
}

//	Exclusive or: true iff exactly one argument is true.
//	The arguments are negated to ensure that they are 0/1
//	values.  Then the bitwise Xor operator may apply.
//	(This idea is due to Michael Baldwin.)
inline bool xorb(bool x, bool y)
{
	return !x ^ !y;
}

// Returns true iff c is strictly to the left of the directed
// line through a to b.
template <class T>
inline bool left(const T* a, const T* b, const T* c)
{
	return area2(a, b, c) < 0;
}

template <class T>
inline bool leftOn(const T* a, const T* b, const T* c)
{
	return area2(a, b, c) <= 0;
}

template <class T>
inline bool collinear(const T* a, const T* b, const T* c)
{
	return area2(a, b, c) == 0;
}

//	Returns true iff ab properly intersects cd: they share
//	a point interior to both segments.  The properness of the
//	intersection is ensured by using strict leftness.
template <class T>
bool intersectProp(const T* a, const T* b, const T* c, const T* d)
{
	// Eliminate improper cases.
	if (collinear(a,b,c) || collinear(a,b,d) ||
		collinear(c,d,a) || collinear(c,d,b))
		return false;
	
	return xorb(left(a,b,c), left(a,b,d)) && xorb(left(c,d,a), left(c,d,b));
}

// Returns T iff (a,b,c) are collinear and point c lies
// on the closed segement ab.
template <class T>
bool between(const T* a, const T* b, const T* c)
{
	if (!collinear(a, b, c))
		return false;
	// If ab not vertical, check betweenness on x; else on y.
	if (a[0] != b[0])
		return	((a[0] <= c[0]) && (c[0] <= b[0])) || ((a[0] >= c[0]) && (c[0] >= b[0]));
	else
		return	((a[2] <= c[2]) && (c[2] <= b[2])) || ((a[2] >= c[2]) && (c[2] >= b[2]));
}

// Returns true iff segments ab and cd intersect, properly or improperly.
template <class T>
bool intersect(const T* a, const T* b, const T* c, const T* d)
{
	if (intersectProp(a, b, c, d))
		return true;
	else if (between(a, b, c) || between(a, b, d) ||
			 between(c, d, a) || between(c, d, b))
		return true;
	else
		return false;
}

template <class T>
bool vequal(const T* a, const T* b)
{
	return a[0] == b[0] && a[2] == b[2];
}

float distancePtSeg(const int x, const int z,
 				    const int px, const int pz,
				    const int qx, const int qz);

#endif
