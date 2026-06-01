/* ARCA CPP-project
 * FILE: resize.h - for resize bonus object
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#pragma once

#include "bonus.h"

class SecondBottomBonus : public Bonus
{
public:
    SecondBottomBonus(const t_vec2& pos);
    ~SecondBottomBonus() = default;

    void Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages) override;
};
