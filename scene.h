/* GEMS CPP-project
 * FILE: scene.h - for match-3 scene management
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#pragma once

#include <vector>
#include "prim.h"

#define FSIZE 30        // grid size: 30x30

 // enum for bonus types
enum BonusType {
    BONUS_RECOLOR,      // recolor bonus
    BONUS_BOMB          // bomb bonus
};

// struct for bonus data
struct Bonus {
    BonusType type;                 // type of bonus (recolor or bomb)
    int sourceIndex;                // index of square where bonus originated
    int targetIndex;                // index of target square
    t_vec3 originalColor;           // original color of the source square
    std::vector<int> bombIndices;   // indices affected by bomb bonus
};

class Primitive;

// class for the game scene, grid, match handling and bonuses
class Scene
{
    int mScrW = 900, mScrH = 900;          // screen width and height
    std::vector<Primitive*> mPrimitives;    // all primitives in the scene
    int mSelectedIndex = -1;                // index of currently selected square

public:
    Scene() = default;
    Scene(int W, int H);
    ~Scene();

    void Display();
    void Keyboard(unsigned char Key, int X, int Y);
    void Timer(int value);
    void Reshape(int w, int h);
    void Mouse(int button, int state, int x, int y);
    void Init(int argc, char** argv);

    static void StaticDisplay();
    static void StaticKeyboard(unsigned char Key, int X, int Y);
    static void StaticTimer(int value);
    static void StaticReshape(int w, int h);
    static void SetCurrentScene(Scene* scene);
    static void StaticMouse(int button, int state, int x, int y);

    void ProcessMatches();
    static void StepMatchTimerCallback(int depth);

private:
    static Scene* mCurrentScene;    // current scene for static callbacks

    void GenerateColorGrid(int grid[FSIZE][FSIZE], const std::vector<t_vec3>& colors);
    bool FindAndMarkMatches();
    void ApplyGravityAndRefill();
    void StepMatchProcessing(int recursionDepth);

    std::vector<t_vec3> mColors;    // color palette
    bool mProcessingMatches;        // flag to avoid recursive match processing

    std::vector<Bonus> mPendingBonuses;
    void TryDropBonus(int row, int col, const t_vec3& color);
    void ApplyPendingBonuses();
};

