//
// Created by Yuanqi on 12/27/16.
//

#ifndef PBR_BOUNDS_H
#define PBR_BOUNDS_H

/**
 * There are a few possible representations for these sorts of bounding boxes; pbr uses axis-aligned bounding boxes
 * (AABBs), where the box edges are mutually perpendicular and aligned with the coordinate system axes. Another possible
 * choice is oriented bounding boxes (OBBs), where the box edges on different sides are still perpendicular to each
 * other. but not necessarily coordinate-system aligned. A 3D AABB can be described by one of its vertices and three
 * lengths. Alternatively, two opposite vertices of the box can describe it.
 */

template <typename T>
class Bounds2 {

public:



};


template <typename T>
class Bounds3 {

public:

    Point3<T> pMin, pMax;

    // The default constructors create an empty box by setting the extent to an invalid configuration, which violates
    // the invariant that pMin.x <= pMax.x (so do other dimensions).

    Bounds3(
    ) {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = Point3<T>(maxNum, maxNum, maxNum);
        pMax = Point3<T>(minNum, minNum, minNum);
    }

    // It's useful to initialize a Bounds3 to enclose a single point.

    Bounds3(
            const Point3<T> &p
    ) : pMin(p),
        pMax(p)
    {
    }

    // If the caller passes two corner points to define the box, the constructor needs to find their component-wise min
    // and max values since p1 and p2 are not necessarily chosen so that p1.x <= p2.x, and so on.

    Bounds3(
            const Point3<T> &p1,
            const Point3<T> &p2
    ) : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
        pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z))
    {
    }

    // Selects between pMin and pMax based on the value of i.

    const Point3<T> &operator[] (int i) const;
    Point3<T> &operator[] (int i);

    // Returns the coordinates of one of the eights corners of the bounding box.

    Point3<T> Corner(int corner) const {
        return Point3<T>((*this)[(corner & 1)].x,
                         (*this)[(corner & 2) ? 1 : 0].y,
                         (*this)[(corner & 4) ? 1 : 0].z);
    }

    // Ray-box intersection predicate method. If it returns true, the intersection's parametric range is returned in the
    // optional arguments hitt0, and hitt1. Intersections outside of the (0, Ray::tMax) range of the ray are ignored. If
    // the ray's origin is inside the box, 0 is returned for hitt0.

    inline bool IntersectP(const Ray &ray, Float *hitt0, Float *hitt1) const {
        Float t0 = 0, t1 = ray.tMax;

        // For each pair of planes, it needs to compute two ray-plane intersections, using the following formula:
        //     t = (x.i - o.i) / d.i;
        // This gives a parametric range [tNear, tFar], which is used to compute the set intersection with the current
        // range [t0, t1] to compute a new range. If this new range is empty (i.e., t0 > t1), then the code can
        // immediately return failure.

        for (int i = 0; i < 3; i++) {

            Float invRayDir = 1 / ray.d[i];
            Float tNear = (pMin[i] - ray.o[i]) * invRayDir;
            Float tFar  = (pMax[i] - ray.o[i]) * invRayDir;

            if (tNear > tFar) std::swap(tNear, tFar);

            // update tFar to ensure robust ray-bounds intersection

            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar  < t1 ? tFar  : t1;

            if (t0 > t1) return false;
        }

        if (hitt0) *hitt0 = t0;
        if (hitt1) *hitt1 = t1;

        return true;
    }

    inline bool IntersectP(const Ray &ray, const Vector3f &invDir, const int dirIsNeg[3]) const {
        const Bounds3f &bounds = *this;
        // Check for ray intersection against x and y slabs
        // Check for ray intersection against z slab
        return tMin < ray.tMax && tMax > 0;
    }

};

typedef Bounds2<Float> Bounds2f;
typedef Bounds2<int>   Bounds2i;
typedef Bounds3<Float> Bounds3f;
typedef Bounds3<int>   Bounds3i;

#endif //PBR_BOUNDS_H
