//
// Created by Ricky on 2016/12/27.
//

#ifndef PBR_SPHERE_H
#define PBR_SPHERE_H

#include "Shape.h"
#include "../core/pbr.h"
#include "../core/Bounds.h"
#include "../core/Ray.h"
#include "../core/Interaction.h"
#include "../core/Point.h"
#include "../core/Transform.h"
#include "../core/Vector.h"

/**
 * The unit sphere is given by x^2 + y^2 + z^2 - 1 = 0.
 */

class Sphere : public Shape {

public:

    const Float radius;
    const Float zMin, zMax;
    const Float thetaMin, thetaMax, phiMax;

    Sphere(const Transform *ObjectToWorld,
           const Transform *WorldToObject,
           bool reverseOrientation,
           Float radius,
           Float zMin,
           Float zMax,
           Float phiMax
    ) : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
        radius(radius),
        zMin(Clamp(std::min(zMin, zMax), -radius, radius)),
        zMax(Clamp(std::max(zMin, zMax), -radius, radius)),
        thetaMin(std::acos(Clamp(zMin / radius, -1, 1))),
        thetaMax(std::acos(Clamp(zMax / radius, -1, 1))),
        phiMax(Radians(Clamp(phiMax, 0, 360)))
    {
    }

    // TODO: It doesn't provide a tighter bounding box when phiMax is less than 3pi/2.

    Bounds3f ObjectBound() const {
        return Bounds3f(Point3f(-radius, -radius, zMin),
                        Point3f( radius,  radius, zMax));
    }

    // If the sphere has been transformed to another position in world space, then it is necessary to transform rays
    // to object space before intersecting them with the sphere, using the world-to-object transformation.

    bool Intersect(const Ray &ray,
                   Float *tHit,
                   SurfaceInteraction *isect,
                   bool testAlphaTexture) const override {
        Float phi;
        Point3f pHit;

        // transform ray to object space

        Vector3f oErr, dErr;
        Ray ray = (*WorldToObject)(ray, &oErr, &dErr);

        // compute quadratic sphere coefficients

        EFloat ox(ray.o.x, oErr.x), oy(ray.o.y, oErr.y), oz(ray.o.z, oErr.z);
        EFloat dx(ray.d.x, dErr.x), dy(ray.d.y, dErr.y), dz(ray.d.z, dErr.z);
        EFloat a = dx * dx + dy * dy + dz * dz;
        EFloat b = 2 * (dx * ox + dy * oy + dz * oz);
        EFloat c = ox * ox + oy * oy + oz * oz - EFloat(radius) * EFloat(radius);

        // solve quadratic equation for t values

        EFloat t0, t1;
        if (!Quadratic(a, b, c, &t0, &t1))
            return false;

        // check quadratic shape t0 and t1 for nearest intersection

        if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0)
            return false;
        EFloat tShapeHit = t0;
        if (tShapeHit.LowerBound() <= 0) {
            tShapeHit = t1;
            if (tShapeHit.UpperBound() > ray.tMax)
                return false;
        }

        // compute sphere hit position and phi
        //     y     r sinθ sinφ
        //    --- = ------------- = tanφ  =>  φ = arctan y/x
        //     x     r sinθ cosφ

        pHit = ray((Float)tShapeHit);
        // refine sphere intersection point
        if (pHit.x == 0 && pHit.y == 0)
            phi.x = 1e-5f * radius;
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0)
            phi += 2 * Pi;

        // test sphere intersection against clipping parameters

        // find parametric representation of sphere hit

        // compute error bounds for sphere intersection

        // initialize SurfaceInteraction from parametric information

        // update tHit for quadratic intersection

        return true;
    }


};






#endif //PBR_SPHERE_H
