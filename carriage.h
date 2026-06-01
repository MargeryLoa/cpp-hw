/* GEMS CPP-project
 * FILE: carriage.h - for car object
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */
#pragma once

#include "prim.h"

 // class for a car primitive (inherits from Primitive)

class Carriage : public Primitive
{
public:
    Carriage(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float W, float H);
    ~Carriage() = default;

    float GetWidth() const { return mWidth; }           // returns long side length
    float GetHeight() const { return mHeight; }           // returns short side length

    void SetWidth(float W) { mWidth = W; }
    void UpdateCar(void);
private:
    float mWidth, mHeight;        // side sizes

};

