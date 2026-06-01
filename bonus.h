 /* ARCA CPP-project
  * FILE: carriage.h - for car object
  * PROGRAMMER: Baydakova M.
  * GROUP: 5030102/40004
  * UPDATE: 31.05.2026
  */
#pragma once

#include "ball.h"
#include "carriage.h"

  // class for a car primitive (inherits from Primitive)
class Bonus
{
public:
    Bonus(const t_vec2& pos);
    Bonus(const t_vec2& pos, const t_vec3& color);

    ~Bonus() = default;

    void Draw() { mBonusBall->Draw(); }
    int CheckCollect(Carriage *C);
    void Live(Carriage *C);
    virtual void Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages) {}

    bool IsToDelete() const { return mToDelete; }
    void SetToDelete(bool del) { mToDelete = del; }

    bool IsToAct() const { return mToAct; }
    void SetToAct(bool act) { mToAct = act; }

    Ball* GetBall() { return mBonusBall; }
private:
    Ball* mBonusBall;
    bool mToDelete, mToAct;
};





