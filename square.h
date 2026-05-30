/* GEMS CPP-project
 * FILE: square.h - for square-shaped primitive
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#pragma once

#include "prim.h"
#include <string>

 // class for a square primitive (inherits from Primitive)
class Square : public Primitive
{
public:
    Square(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float sideSize);
    ~Square();

    float GetSize() const { return mSize; }           // returns side length
    const std::string& GetName() const { return mName; } // returns shape name

private:
    float mSize;           // side length
    std::string mName;     // always "SQUARE"
};

