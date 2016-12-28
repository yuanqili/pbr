//
// Created by Ricky on 2016/12/27.
//

#ifndef PBR_VECTOR_H
#define PBR_VECTOR_H

#include "pbr.h"

template <typename T>
class Vector {

public:

    T x, y, z;

};

typedef Vector<Float> Vector3f;
typedef Vector<int>   Vector3i;


#endif //PBR_VECTOR_H
