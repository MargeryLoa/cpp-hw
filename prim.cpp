/* GEMS CPP-project
 * FILE: prim.cpp - implementation of base primitive class
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#include "prim.h"
#include "glut.h"
#include <algorithm>

 /* Function for Primitive constructor
  * ARGS:
  *  (const t_vec2&) - initial position
  *  (const t_vec2&) - initial speed
  *  (const t_vec3&) - color
  *  (const std::vector<t_vec2>&) - local vertex coordinates
  * RETS: None.
  */
Primitive::Primitive(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, const std::vector<t_vec2>& vertices)
    : mPosition(pos), mSpeed(speed), mColor(color), mVertex(vertices), mToDraw(true)
{
}

/* Function for Primitive destructor
 * ARGS: None.
 * RETS: None.
 */
Primitive::~Primitive()
{
}

/* Function for updating position and handling window boundaries
 * ARGS:
 *  (int) - window width
 *  (int) - window height
 * RETS: None.
 */
void Primitive::Update(int windowWidth, int windowHeight)
{
    mPosition.x += mSpeed.x;
    mPosition.y += mSpeed.y;

    float minX = mVertex[0].x, maxX = mVertex[0].x;
    float minY = mVertex[0].y, maxY = mVertex[0].y;
    for (const auto& v : mVertex) {
        if (v.x < minX) minX = v.x;
        if (v.x > maxX) maxX = v.x;
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }
    float halfW = (maxX - minX) / 2.0f;
    float halfH = (maxY - minY) / 2.0f;

    if (mPosition.x - halfW < 0) {
        mPosition.x = halfW;
        mSpeed.x = -mSpeed.x;
    }
    if (mPosition.x + halfW > windowWidth) {
        mPosition.x = windowWidth - halfW;
        mSpeed.x = -mSpeed.x;
    }

    if (mPosition.y - halfH < 0) {
        mPosition.y = halfH;
        mSpeed.y = -mSpeed.y;
    }
    if (mPosition.y + halfH > windowHeight) {
        mPosition.y = windowHeight - halfH;
        mSpeed.y = -mSpeed.y;
    }
}

/* Function for drawing the primitive
 * ARGS: None.
 * RETS: None.
 */
void Primitive::Draw() const
{
    glColor3f(mColor.r / 255.0f, mColor.g / 255.0f, mColor.b / 255.0f);

    glBegin(GL_POLYGON);
    for (const auto& v : mVertex) {
        glVertex2f(mPosition.x + v.x, mPosition.y + v.y);
    }
    glEnd();
}

