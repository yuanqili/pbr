//
// Created by Yuanqi on 12/26/16.
//

#ifndef PBR_SHAPE_H
#define PBR_SHAPE_H

/**
 * The Shape class provides access to the raw geometric properties of the primitive, such as its surface area and
 * bounding box, and provides a ray intersection routine.
 */

class Shape {

public:

    // All shapes are defined in object coordinate space. In order to place a sphere at another position in the scene,
    // a transformation that describes the mapping from object space to world space must be provided.
    // An important detail is that shapes store only pointers to their transformations because many objects may share
    // the same transformation.

    const Transform *ObjectToWorld, *WorldToObject;
    const bool reverseOrientation;
    const bool transformSwapsHandedness;

    Shape(
            const Transform *ObjectToWorld,
            const Transform *WorldToObject,
            bool reverseOrientation
    ) : ObjectToWorld(ObjectToWorld),
        WorldToObject(WorldToObject),
        reverseOrientation(reverseOrientation),
        transformSwapsHandedness(ObjectToWorld->SwapsHandedness())
    {
    }

    // For many operations, it is often useful to have a 3D bounding volume that encloses an object.
    // Each Shape implementation must therefore be capable of bounding itself with an axis-aligned bounding box
    // represented by a Bound3f.

    // Returns a bounding box in shape's object space.

    virtual Bounds3f ObjectBound() const = 0;

    // Returns a bounding box in world space.

    Bounds3f WorldBound() const {
        return (*ObjectToWorld)(ObjectBound());
    }

    // Returns geometric information about a single ray-shape intersection corresponding to the first intersection,
    // if any, in the (0, tMax) parametric range along the ray. Several things to be noticed:
    //
    // - The Ray structure contains a Ray::tMax member that defines the endpoint of the ray.
    // - If an intersection is found, its parametric distance along the ray should be stored in the tHit pointer.
    // - Information about an intersection is stored in the SurfaceInteraction structure.
    // - The rays passed into intersection routines are in world space, so shapes are responsible for transforming them
    //   to object space if needed for intersection tests. The intersection information returned should be in world
    //   space.
    //
    // Some Shape implementation support cutting away some of their surfaces using a texture; the testAlphaTexture
    // parameter indicates whether those that do should perform this operation for the current intersection test.

    virtual bool Intersect(
            const Ray &ray,
            Float *tHit,
            SurfaceInteraction *isect,
            testAlphaTexture = true
    ) const = 0;

    // A predicate function determines whether or not an intersection occurs, without returning any details about the
    // intersection itself. This serves as the default implementation of this operation. Almost all shape
    // implementations provide a more efficient implementation for it.

    virtual bool IntersectP(
            const Ray &ray,
            bool testAlphaTexture = true
    ) const {
        Float tHit = ray.tMax;
        SurfaceInteraction isect;
        return Intersect(ray, &tHit, &isect, testAlphaTexture);
    }

    // In order to properly use Shapes as area lights, it is necessary to be able to compute the surface area of a shape
    // in object space.

    virtual Float Area() const = 0;

};


#endif //PBR_SHAPE_H
