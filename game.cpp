/* GEMS CPP-project
 * FILE: game.h - for game logic implementation
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#define IsIn(A, B, E) (A >= B && A <= E)

#include "game.h"
#include "bonus.h"
#include "stuck.h"
#include "snd_bottom.h"
#include "snd_ball.h"
#include "resize.h"
#include "sp_down.h"


#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstdio>

Game::Game(int w, int h, const std::vector<t_vec3>& BlockPalette) 
{ 
    (void)w; 
    (void)h; 
    mBlockPalette = BlockPalette;
}

Game::~Game() {
    for (auto* sq : mBlocks) delete sq;
    for (auto* sq : mCars) delete sq;
    for (auto* sq : mBalls) delete sq;
}

/* Function for initializing the grid with squares using pre‑generated color indices
 * ARGS:
 *  (float) - start X coordinate (leftmost center)
 *  (float) - start Y coordinate (bottommost center)
 *  (const std::vector<t_vec3>&) - color palette
 * RETS: None.
 */
void Game::InitObjects(float startX, float startY, t_vec2 CarPos, const std::vector<t_vec3>& BlockPalette, int FloorLength)
{
    mPlayer = new Player();

    float stepW = BLOCK_W + DIST;
    float stepH = BLOCK_H + DIST;
    
    for (int row = 0; row < GRID_H; ++row) {
        for (int col = 0; col < GRID_W; ++col) {
            t_vec2 pos = { startX + col * stepW, startY + row * stepH };

            int Health = rand() % 3 + 2;
            int TypeU = rand() % 10;
            if (TypeU)
                mBlocks.push_back(new Block(pos, BlockPalette[Health], BLOCK_W, BLOCK_H, (BlockType)(rand() % 3 + 1), Health+1));
            else 
                mBlocks.push_back(new Block(pos, BlockPalette[5], BLOCK_W, BLOCK_H, UNBREAKING, Health + 1));
        }
    }

    mCars.push_back(new Carriage(CarPos, { 15, 0 }, { 0, 0, 100 }, 2 * BLOCK_W, 1.5 * BLOCK_H));
    mCars.push_back(new Carriage({0, 100}, {0, 0}, {0, 0, 50}, FloorLength, BLOCK_H));
    mCars[1]->SetToDraw(0);
    mBalls.push_back(new Ball({450, 450}, {5, 5}, {200, 0, 0}, 15, 8));
}

void Game::MoveCar(int flag, int Board)
{
    t_vec2 Pos = mCars[0]->GetPosition(),
           Speed = mCars[0]->GetSpeed();

    if ((flag == 1 && Pos.x <= Board - mCars[0]->GetWidth()) || (flag == -1 && Pos.x >= Board))
        Pos.x += (Speed.x * flag);

    mCars[0]->SetPosition(Pos);
}

int Game::CheckTouch(t_vec2 BallPos, t_vec2 BallSpeed, t_vec2 ObjectPos, float Radius, float W, float H)
{
    if (IsIn(BallPos.x, ObjectPos.x, ObjectPos.x + W))
    {
        if (BallSpeed.y <= 0 && BallPos.y == ObjectPos.y + H + Radius)
            return 1;
        else if (BallSpeed.y >= 0 && BallPos.y == ObjectPos.y - Radius)
            return 1;
    }

    if (IsIn(BallPos.y, ObjectPos.y, ObjectPos.y + H))
    {
        if (BallSpeed.x <= 0 && BallPos.x == ObjectPos.x + W + Radius)
            return -1;
        else if (BallSpeed.x >= 0 && BallPos.x == ObjectPos.x - Radius)
            return -1;
    }
    return 0;
}

static int CheckAndResolveCollision(t_vec2& center, float radius,
    const t_vec2& rectPos, float w, float h)
{
    float closestX = std::max(rectPos.x, std::min(center.x, rectPos.x + w));
    float closestY = std::max(rectPos.y, std::min(center.y, rectPos.y + h));
    float dx = center.x - closestX;
    float dy = center.y - closestY;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist >= radius) return 0;

    float overlap = radius - dist;
    if (dist < 0.0001f) dist = 0.0001f;
    float nx = dx / dist;
    float ny = dy / dist;
    center.x += nx * overlap;
    center.y += ny * overlap;
    if (std::fabs(dx) > std::fabs(dy))
        return -1;
    else
        return 1; 
}

void Game::CheckCollideCar()
{
    for (size_t i = 0; i < mBalls.size(); ++i)
    {
        for (size_t j = 0; j < mCars.size(); ++j)
        {
            if (!mCars[j]->IsToDraw()) continue;

            t_vec2 ballPos = mBalls[i]->GetPosition();
            int side = CheckAndResolveCollision(ballPos, mBalls[i]->GetRadius(),
                mCars[j]->GetPosition(),
                mCars[j]->GetWidth(), mCars[j]->GetHeight());
            if (side != 0)
            {
                mBalls[i]->SetPosition(ballPos);
                if (j == 1) mCars[j]->SetToDraw(false); 

                t_vec2 spd = mBalls[i]->GetSpeed();
                if (side == 1)
                    mBalls[i]->SetSpeed({ spd.x, -spd.y });
                else
                    mBalls[i]->SetSpeed({ -spd.x, spd.y });
            }
        }
    }
}

void Game::ThrowBonus(const t_vec2& pos)
{
    int BT = rand() % 5;

    switch (BT)
    {
    case 0:
        mBonuses.push_back(new ResizeBonus(pos));
        break;
    case 1:
        mBonuses.push_back(new SpeedDownBonus(pos)); 
        break;
    case 2:
        mBonuses.push_back(new SecondBallBonus(pos));
        break;
    case 3:
        mBonuses.push_back(new SecondBottomBonus(pos));
        break;
    case 4:
        mBonuses.push_back(new StuckBonus(pos));
        break;
    }
}

void Game::CollideBlock(int BIndex, int BlockIndex)
{
    BlockType T = mBlocks[BlockIndex]->GetType();
    if (T != UNBREAKING)
    {
        int H = mBlocks[BlockIndex]->GetHealth() - 1;
        mBlocks[BlockIndex]->SetHealth(H);

        if (mBlocks[BlockIndex]->GetHealth() == 0)
        {
            mBlocks[BlockIndex]->SetToDraw(0);
            if (T == BONUS)
            {
                t_vec2 Pos = mBlocks[BlockIndex]->GetPosition();
                Pos.y -= (mBlocks[BlockIndex]->GetWidth() / 2);
                ThrowBonus(Pos);
            }//Falls Bonus;
        }
        else
            mBlocks[BlockIndex]->SetColor(mBlockPalette[H - 1]);

        if (T == SPEEDUP)
        {
            t_vec2 OldSpeed = mBalls[BIndex]->GetSpeed();

            OldSpeed.x *= 1.01;
            OldSpeed.y *= 1.01;

            mBalls[BIndex]->SetSpeed(OldSpeed);
        }
    }
}

void Game::CheckCollideBlock()
{
    for (size_t i = 0; i < mBalls.size(); ++i)
    {
        for (size_t j = 0; j < mBlocks.size(); ++j)
        {
            if (!mBlocks[j]->IsToDraw()) continue;

            t_vec2 ballPos = mBalls[i]->GetPosition();
            int side = CheckAndResolveCollision(ballPos, mBalls[i]->GetRadius(),
                mBlocks[j]->GetPosition(),
                mBlocks[j]->GetWidth(), mBlocks[j]->GetHeight());
            if (side != 0)
            {
                mBalls[i]->SetPosition(ballPos);

                CollideBlock(static_cast<int>(i), static_cast<int>(j));
                mPlayer->ChangeScore(1);

                t_vec2 spd = mBalls[i]->GetSpeed();
                if (side == 1)
                    mBalls[i]->SetSpeed({ spd.x, -spd.y });
                else if (side == -1)
                    mBalls[i]->SetSpeed({ -spd.x, spd.y });
            }
        }
    }
}
void Game::CheckLoose()
{
    for (int i = 0; i < mBalls.size(); i++)
        if (mBalls[i]->GetPosition().y == mBalls[i]->GetRadius())
            mPlayer->ChangeScore(-1);
}

void Game::FreeBonus()
{
    for (auto it = mBonuses.begin(); it != mBonuses.end(); )
    {
        if ((*it)->IsToDelete())
        {
            delete* it;              
            it = mBonuses.erase(it); 
        }
        else
            ++it;
    }
}

void Game::ActBonus()
{
    for (int i = 0; i < mBonuses.size(); i++)
        if (mBonuses[i]->IsToAct())
            mBonuses[i]->Act(mBalls, mCars);
}


void Game::CheckCollide2Balls()
{
    for (int i = 0; i < mBalls.size(); i++)
        for (int j = i + 1; j < mBalls.size(); j++) 
        {
            Ball* ball1 = mBalls[i];
            Ball* ball2 = mBalls[j];

            t_vec2 pos1 = ball1->GetPosition();
            t_vec2 pos2 = ball2->GetPosition();
            float r1 = ball1->GetRadius();
            float r2 = ball2->GetRadius();

            float dx = pos2.x - pos1.x;
            float dy = pos2.y - pos1.y;
            float dist = sqrt(dx * dx + dy * dy);
            float minDist = r1 + r2;

            if (dist < minDist)
            {
                float nx = dx / dist;
                float ny = dy / dist;

                t_vec2 v1 = ball1->GetSpeed();
                t_vec2 v2 = ball2->GetSpeed();

                float v_rel_n = (v2.x - v1.x) * nx + (v2.y - v1.y) * ny;

                if (v_rel_n < 0)
                {
                    float e = 1.0f;
                    float imp = (1 + e) * v_rel_n / 2.0f;

                    ball1->SetSpeed({ v1.x + imp * nx, v1.y + imp * ny });
                    ball2->SetSpeed({ v2.x - imp * nx, v2.y - imp * ny });
                }

                float overlap = minDist - dist;
                float correction = overlap / 2.0f;
                ball1->SetPosition({ pos1.x - nx * correction, pos1.y - ny * correction });
                ball2->SetPosition({ pos2.x + nx * correction, pos2.y + ny * correction });
            }
        }
}

