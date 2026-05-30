#pragma once
#include "prim.h"

#include <vector>

class Game; // forward declaration

class Bonus {
public:
    virtual ~Bonus() = default;
    virtual void Apply(Game* game) const = 0;
};

