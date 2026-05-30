#pragma once
#include "prim.h"
#include "bonus.h"
#include <vector>

class Game; // forward declaration

//class Bonus;

class BombBonus : public Bonus {
    std::vector<int> m_indices;
public:
    BombBonus(const std::vector<int>& indices);
    void Apply(Game* game) const override;
};

