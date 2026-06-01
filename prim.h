/* GEMS CPP-project
 * FILE: prim.h - for base primitive class and vector types
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#pragma once

#include <vector>
#include <string>

 // struct for 2D vector coordinates
typedef struct {
    float x, y;             // X and Y components
} t_vec2;

// struct for RGB color components
typedef struct {
    unsigned char r, g, b;  // red, green, blue values 0..255
} t_vec3;

// class for a drawable moving primitive with vertices
class Primitive
{
    bool mToDraw;           // flag indicating whether primitive should be drawn

public:
    Primitive(const t_vec2& pos, const t_vec2& speed, const t_vec3& color, const std::string& name, const std::vector<t_vec2>& vertices);
    Primitive(const t_vec2& pos, const t_vec3& color, const std::string& name, const std::vector<t_vec2>& vertices);

    virtual ~Primitive() = default;

    void Update(int windowWidth, int windowHeight);
    void Draw() const;

    void SetSpeed(const t_vec2& speed) { mSpeed = speed; }
    t_vec2 GetSpeed() const { return mSpeed; }

    void SetColor(const t_vec3& color) { mColor = color; }
    t_vec3 GetColor() const { return mColor; }

    void SetPosition(const t_vec2& pos) { mPosition = pos; }
    t_vec2 GetPosition() const { return mPosition; }

    const std::string& GetName() const { return mName; } // returns shape name

    bool IsToDraw() const { return mToDraw; }
    void SetToDraw(bool draw) { mToDraw = draw; }

protected:
    void SetVertices(const std::vector<t_vec2>& vertices) { mVertex = vertices; }

    t_vec2 mPosition;               // current position
    t_vec2 mSpeed;                  // velocity vector
    t_vec3 mColor;                  // color
    std::string mName;              // primitive type BALL|CAR|BLOCK
    std::vector<t_vec2> mVertex;    // local vertices relative to position
};

