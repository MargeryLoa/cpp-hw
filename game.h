/* GEMS CPP-project
 * FILE: game.h - for game logic class
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#pragma once
#include <vector>
#include "prim.h"
#include "block.h"
#include "ball.h"
#include "carriage.h"
#include "player.h"
#include "bonus.h"

class Primitive;

class Game {
public:
    static constexpr int GRID_W = 10, GRID_H = 10;
    static constexpr int BLOCK_W = 80, BLOCK_H = 20, DIST = 4;

    Game(int screenW, int screenH, const std::vector<t_vec3>& BlockPalette);
    ~Game();

    void InitObjects(float startX, float startY, t_vec2 CarPos, const std::vector<t_vec3>& BlockPalette, int FloorLength);

    std::vector<Block*>& GetBlocks() { return mBlocks; }
    std::vector<Ball*>& GetBalls() { return mBalls; }
    std::vector<Carriage*>& GetCars() { return mCars; }
    std::vector<Bonus*>& GetBonuses() { return mBonuses; }
    Player* GetPlayer() { return mPlayer; }

    void MoveCar(int flag, int Board);
    
    void ThrowBonus(const t_vec2& pos);

    void CheckCollideCar();
    void CheckCollideBlock();
    void CheckLoose();
    void CollideBlock(int BIndex, int BlockIndex);
    void CheckCollide2Balls();

    void FreeBonus();
    void ActBonus();
    
    static int CheckTouch(t_vec2 BallPos, t_vec2 BallSpeed, t_vec2 ObjectPos, float Radius, float W, float H);
private:
    std::vector<Block*> mBlocks;
    std::vector<Ball*> mBalls;          // no more than 2
    std::vector<Carriage*> mCars;      // no more than 2
    std::vector<Bonus*> mBonuses;

    bool m_processing = false;
    std::vector<t_vec3> mBlockPalette;
    Player* mPlayer = nullptr;
};

