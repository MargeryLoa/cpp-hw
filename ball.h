 /* ARCA CPP-project
  * FILE: block.h - for block object
  * PROGRAMMER: Baydakova M.
  * GROUP: 5030102/40004
  * UPDATE: 31.05.2026
  */

#include "prim.h"


  /* GEMS CPP-project
   * FILE: carriage.h - for car object
   * PROGRAMMER: Baydakova M.
   * GROUP: 5030102/40004
   * UPDATE: 31.05.2026
   */
#pragma once

#include "prim.h"

// class for a car primitive (inherits from Primitive)
class Ball : public Primitive
{
public:
    Ball(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float R, int slices);
    ~Ball() = default;

    float GetRadius() const { return mRadius; }           // returns long side length

private:
    float mRadius;        // side sizes

};
