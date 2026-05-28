/* GEMS CPP-project
 * FILE: square.cpp - implementation of square primitive
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#include "square.h"
#include <vector>

 /* Function for Square constructor
  * ARGS:
  *  (const t_vec2&) - initial position
  *  (const t_vec2&) - initial speed
  *  (const t_vec3&) - color
  *  (float) - side size
  * RETS: None.
  */
Square::Square(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float sideSize)
    : Primitive(pos, speed, color, {}), mSize(sideSize), mName("SQUARE")
{
    float half = sideSize / 2.0f;
    std::vector<t_vec2> vertices = {
        { -half, -half },
        {  half, -half },
        {  half,  half },
        { -half,  half }
    };
    SetVertices(vertices);
}

/* Function for Square destructor
 * ARGS: None.
 * RETS: None.
 */
Square::~Square() {}

