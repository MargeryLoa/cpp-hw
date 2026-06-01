/* GEMS CPP-project
 * FILE: block.h - for block object
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#include "prim.h"

enum BlockType
{
    UNBREAKING,
    SIMPLE,
    BONUS,
    SPEEDUP
};

// class for a square primitive (inherits from Primitive)
class Block : public Primitive
{
public:
    Block(const t_vec2& pos, const t_vec3& color, float W, float H, BlockType Type, int Health);
    ~Block() = default;

    float GetWidth() const { return mWidth; }           // returns long side length
    float GetHeight() const { return mHeight; }           // returns short side length
    
    void SetHealth(int H) { mBlockHealth = H; }
    int GetHealth() const { return mBlockHealth; }

    BlockType GetType() { return mType; }
private:
    float mWidth, mHeight;        // side sizes
    BlockType mType;
    int mBlockHealth;
};

