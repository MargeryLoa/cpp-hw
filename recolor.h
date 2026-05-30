#pragma once
#include "prim.h"
#include "bonus.h"
#include <vector>

class Game; // forward declaration

//class Bonus;

class RecolorBonus : public Bonus {
    int m_targetIndex;
    t_vec3 m_newColor;
    std::vector<int> m_extraIndices;
public:
    RecolorBonus(int target, const t_vec3& color, const std::vector<int>& extra);
    void Apply(Game* game) const override;
};

