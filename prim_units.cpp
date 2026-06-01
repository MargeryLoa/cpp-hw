/* GEMS CPP-project
 * FILE: square.cpp - implementation of primitive units
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#include "glut.h"

#include "block.h"
#include "ball.h"
#include "carriage.h"

#include <vector>

 /* Function for Block constructor
  * ARGS:
  *  (const t_vec2&) - initial position
  *  (const t_vec2&) - initial speed
  *  (const t_vec3&) - color
  *  (float) - side size
  * RETS: None.
  */
Block::Block(const t_vec2& pos, const t_vec3& color, float W, float H, BlockType Type, int Health)
    : Primitive(pos, color, "BLOCK", {}), mWidth(W), mHeight(H), mType(Type)
{
    std::vector<t_vec2> vertices = {
        { 0, 0 },
        {  W, 0 },
        {  W,  H },
        { 0,  H }
    };
    SetVertices(vertices);
    mBlockHealth = Health;
}

/* Function for Car constructor
 * ARGS:
 *  (const t_vec2&) - initial position
 *  (const t_vec2&) - initial speed
 *  (const t_vec3&) - color
 *  (float) - side size
 * RETS: None.
 */
Carriage::Carriage(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float W, float H)
    : Primitive(pos, speed, color, "CAR", {}), mWidth(W), mHeight(H)
{
    std::vector<t_vec2> vertices = {
        { 0, 0 },
        {  W, 0 },
        {  W,  H },
        { 0,  H }
    };
    SetVertices(vertices);
}

/* Function for Ball constructor
 * ARGS:
 *  (const t_vec2&) - initial position
 *  (const t_vec2&) - initial speed
 *  (const t_vec3&) - color
 *  (float) - side size
 * RETS: None.
 */
Ball::Ball(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, float R, int slices)
    : Primitive(pos, speed, color, "BALL", {}), mRadius(R)
{
    std::vector<t_vec2> vertices;
    for (int i = 0; i < slices; ++i) {
        float angle = 2.0f * 3.1415926535 * i / slices;
        float x = R * cosf(angle);
        float y = R * sinf(angle);
        vertices.push_back({ x, y });
    }
    SetVertices(vertices);
}

void Carriage::UpdateCar(void)
{
    float W = GetWidth(), H = GetHeight();
    std::vector<t_vec2> vertices = {
    { 0, 0 },
    {  W, 0 },
    {  W,  H },
    { 0,  H }
    };
    SetVertices(vertices);
}