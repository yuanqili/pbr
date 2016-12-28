//
// Created by Ricky on 2016/12/27.
//

#ifndef PBR_POINT_H
#define PBR_POINT_H

#include "pbr.h"

template <typename T>
class Point {

public:

    T x, y, z;

    Point(
    ) : x(T(0)), y(T(0)), z(T(0))
    {
    }

    Point(const Float x,
          const Float y,
          const Float z
    ) : x(x), y(y), z(z)
    {
    }

};

typedef Point<Float> Point3f;
typedef Point<int>   Point3i;

#endif //PBR_POINT_H
