//
// Created by Yuanqi on 12/27/16.
//

#ifndef PBR_RAY_H
#define PBR_RAY_H

/**
 * A ray is a semi-infinite line specified by its origin and direction. The parametric form of a ray expresses it as a
 * function of a scalar value t, giving the set of points that the ray passes through:
 *
 *     r(t) = o + td, 0 < t < infty
 */

class Ray {

public:

    // We only need rays with floating-point origins and directions.

    Point3f o;
    Vecter3f d;

    // tMax limits the ray to a segment along its infinite extent, i.e., [o, r(tMax)]

    mutable Float tMax;

    // Each ray has a time value associated with it. In scenes with animated objects, the rendering system constructs a
    // representation of the scene at the appropriate time for each ray.

    Float time;

    // The Medium class encapsulates the properties of media such as a foggy atmosphere, smoke, or scattering liquids
    // like milk or shampoo.

    const Medium *medium;

    // Constructors.

    Ray(
    ) : tMax(Infinity),
        time(0.f),
        medium(nullptr)
    {
    }

    Ray(
            const Point3f &o,
            const Vector3f &d,
            Float tMax = Infinity,
            Float time = 0.f,
            const Medium *medium = nullptr
    ) : o(o),
        d(d),
        tMax(tMax),
        time(time),
        medium(medium)
    {
    }

    // The position along a ray can be thought of as a function of a single parameter t, the Ray class overloads the
    // function application operator for rays.

    Point3f operator()(Float t) const {
        return o + d * t;
    }

};

/**
 * RayDifferential is a subclass of Ray that contains additional information about two auxiliary rays. These extra rays
 * represent camera rays offset by one sample in the x and y direction from the main ray on the film plane. By
 * determining the area that these three rays project to on an object being shaded, the Texture cna estimate an area to
 * average over for proper antialiasing.
 */

class RayDifferential : public Ray {

public:

    bool hasDifferentials;
    Point3f rxOrigin, ryOrigin;
    Vector3f rxDirection, ryDirection;

    RayDifferential(
    ) : hasDifferentials(false)
    {
    }

    RayDifferential(
            const Point3f &o,
            const Vector3f &d,
            Float tMax = Infinity,
            Float time = 0.f,
            const Medium *medium = nullptr
    ) : Ray(o, d, tMax, time, medium),
        hasDifferentials (false)
    {
    }

    // Camera implementations in pbr compute differentials for rays leaving the camera under the assumption that camera
    // rays are spaced one pixel apart.

    void ScaleDifferentials(Float s) {
        rxOrigin = o + (rxOrigin - o) * s;
        ryOrigin = o + (ryOrigin - o) * s;
        rxDirection = d + (rxDirection - d) * s;
        ryDirection = d + (ryDirection - d) * s;
    }

};

#endif //PBR_RAY_H
