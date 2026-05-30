#include "bonus.h"
#include "bomb.h"
#include "recolor.h"
#include "game.h"

RecolorBonus::RecolorBonus(int target, const t_vec3& color, const std::vector<int>& extra)
    : m_targetIndex(target), m_newColor(color), m_extraIndices(extra) {}

void RecolorBonus::Apply(Game* game) const {
    game->RecolorSquare(m_targetIndex, m_newColor);
    for (int idx : m_extraIndices)
        game->RecolorSquare(idx, m_newColor);
}

BombBonus::BombBonus(const std::vector<int>& indices) : m_indices(indices) {}

void BombBonus::Apply(Game* game) const {
    for (int idx : m_indices)
        game->DestroySquare(idx);
}

