/* GEMS CPP-project
 * FILE: scene.cpp - implementation of match-3 scene logic
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#include "glut.h"
#include <cstdlib>
#include <ctime>
#include <cmath>   
#include <algorithm>
#include "scene.h"
#include "prim.h"
#include "square.h"

Scene* Scene::mCurrentScene = nullptr;

/* Function for setting the current scene for static callbacks
 * ARGS:
 *  (Scene*) - pointer to the active scene
 * RETS: None.
 */
void Scene::SetCurrentScene(Scene* scene) { mCurrentScene = scene; }

/* Function for Scene constructor with dimensions
 * ARGS:
 *  (int) - window width
 *  (int) - window height
 * RETS: None.
 */
Scene::Scene(int W, int H) : mScrW(W), mScrH(H) {}

/* Function for Scene destructor
 * ARGS: None.
 * RETS: None.
 */
Scene::~Scene()
{
    for (auto* p : mPrimitives) delete p;
    mPrimitives.clear();
}

/* Function for generating a color grid without three identical colors in a row
 * ARGS:
 *  (int[FSIZE][FSIZE]) - output grid of color indices
 *  (const std::vector<t_vec3>&) - color palette
 * RETS: None.
 */
void Scene::GenerateColorGrid(int grid[FSIZE][FSIZE], const std::vector<t_vec3>& colors)
{
    for (int i = 0; i < FSIZE; ++i)
        for (int j = 0; j < FSIZE; ++j)
            grid[i][j] = -1;

    auto isValid = [&](int row, int col, int idx) -> bool {
        if (col >= 2 && grid[row][col - 1] == idx && grid[row][col - 2] == idx)
            return false;
        if (row >= 2 && grid[row - 1][col] == idx && grid[row - 2][col] == idx)
            return false;
        return true;
        };

    for (int row = 0; row < FSIZE; ++row) {
        for (int col = 0; col < FSIZE; ++col) {
            std::vector<int> available;
            for (size_t i = 0; i < colors.size(); ++i)
                available.push_back((int)i);
            std::random_shuffle(available.begin(), available.end());

            int chosen = -1;
            for (int idx : available) {
                if (isValid(row, col, idx)) {
                    chosen = idx;
                    break;
                }
            }
            if (chosen == -1) chosen = available[0];
            grid[row][col] = chosen;
        }
    }
}

/* Function for initializing GLUT, creating window and grid
 * ARGS:
 *  (int) - argument count
 *  (char**) - argument array
 * RETS: None.
 */
void Scene::Init(int argc, char** argv)
{
    SetCurrentScene(this);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(mScrW, mScrH);
    glutCreateWindow("MB5 - 2D Scene with Primitives");

    glutDisplayFunc(StaticDisplay);
    glutKeyboardFunc(StaticKeyboard);
    glutReshapeFunc(StaticReshape);
    glutTimerFunc(16, StaticTimer, 0);
    glutMouseFunc(StaticMouse);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glClearColor(0.90f, 0.87f, 0.88f, 1.0f);

    float minSide = (mScrW < mScrH) ? mScrW : mScrH;
    float squareSize = minSide * 0.8f / (float)FSIZE;
    float spacing = squareSize * 0.1f;
    float step = squareSize + spacing;
    float totalSpan = (FSIZE - 1) * step;
    float startX = (mScrW - totalSpan - squareSize) / 2.0f + squareSize / 2.0f;
    float startY = (mScrH - totalSpan - squareSize) / 2.0f + squareSize / 2.0f;

    std::vector<t_vec3> colors = {
        {128, 0, 32},    // dark burgundy
        {176, 48, 96},   // medium burgundy
        {220, 100, 140}, // light burgundy
        {75, 0, 130},    // dark purple
        {138, 43, 226},  // purple
        {200, 150, 255}, // light purple
        {85, 107, 47},   // olive
        {204, 85, 0},    // dark orange
        {139, 69, 19}    // brown
    };

    mColors = colors;

    int colorGrid[FSIZE][FSIZE];
    GenerateColorGrid(colorGrid, colors);

    srand((unsigned)time(nullptr));

    for (int row = 0; row < FSIZE; ++row) {
        for (int col = 0; col < FSIZE; ++col) {
            t_vec2 pos = { startX + col * step, startY + row * step };
            t_vec2 speed = { 0.0f, 0.0f };
            t_vec3 color = colors[colorGrid[row][col]];
            Square* square = new Square(pos, speed, color, squareSize);
            mPrimitives.push_back(square);
        }
    }

    glutMainLoop();
}

/* Function for handling window reshape
 * ARGS:
 *  (int) - new width
 *  (int) - new height
 * RETS: None.
 */
void Scene::Reshape(int w, int h)
{
    mScrW = w;
    mScrH = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* Function for periodic updates (timer callback)
 * ARGS:
 *  (int) - unused timer value
 * RETS: None.
 */
void Scene::Timer(int value)
{
    for (auto* p : mPrimitives) {
        p->Update(mScrW, mScrH);
    }

    glutPostRedisplay();
    glutTimerFunc(16, StaticTimer, 0);
}

/* Function for rendering the scene
 * ARGS: None.
 * RETS: None.
 */
void Scene::Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (const auto* p : mPrimitives) {
        if (p->IsToDraw())
            p->Draw();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, mScrH - 20);
    const char* msg = "2D Scene | ESC exit, +/- speed";
    for (const char* c = msg; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glutSwapBuffers();
}

/* Function for handling keyboard input
 * ARGS:
 *  (unsigned char) - key pressed
 *  (int) - mouse X coordinate (unused)
 *  (int) - mouse Y coordinate (unused)
 * RETS: None.
 */
void Scene::Keyboard(unsigned char Key, int X, int Y)
{
    if (Key == 27)
        exit(0);

    if (!mPrimitives.empty()) {
        t_vec2 speed = mPrimitives[0]->GetSpeed();
        if (Key == '+') {
            speed.x += 0.002f;
            speed.y += 0.002f;
            mPrimitives[0]->SetSpeed(speed);
        }
        else if (Key == '-') {
            speed.x -= 0.002f;
            speed.y -= 0.002f;
            mPrimitives[0]->SetSpeed(speed);
        }
    }
}

/* Function for handling mouse clicks (square selection and swapping)
 * ARGS:
 *  (int) - mouse button
 *  (int) - button state (pressed/released)
 *  (int) - mouse X coordinate (window coordinates)
 *  (int) - mouse Y coordinate (window coordinates)
 * RETS: None.
 */
void Scene::Mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    int mouseX = x;
    int mouseY = mScrH - y;

    int hitIndex = -1;
    for (size_t i = 0; i < mPrimitives.size(); ++i) {
        Square* sq = static_cast<Square*>(mPrimitives[i]);
        t_vec2 pos = sq->GetPosition();
        float sz = sq->GetSize();
        float left = pos.x - sz / 2.0f;
        float right = pos.x + sz / 2.0f;
        float bottom = pos.y - sz / 2.0f;
        float top = pos.y + sz / 2.0f;
        if (mouseX >= left && mouseX <= right && mouseY >= bottom && mouseY <= top) {
            hitIndex = (int)i;
            break;
        }
    }

    if (hitIndex == -1) {
        mSelectedIndex = -1;
        return;
    }

    if (mSelectedIndex == -1) {
        mSelectedIndex = hitIndex;
        int row = hitIndex / FSIZE;
        int col = hitIndex % FSIZE;
        printf("Selected square %d (X: %d, Y: %d)\n", hitIndex, col, row);
    }
    else {
        int idx1 = mSelectedIndex;
        int idx2 = hitIndex;
        int row1 = idx1 / FSIZE, col1 = idx1 % FSIZE;
        int row2 = idx2 / FSIZE, col2 = idx2 % FSIZE;

        bool isNeighbor = (std::abs(row1 - row2) == 1 && col1 == col2) ||
            (std::abs(col1 - col2) == 1 && row1 == row2);

        if (isNeighbor) {
            Square* sq1 = static_cast<Square*>(mPrimitives[idx1]);
            Square* sq2 = static_cast<Square*>(mPrimitives[idx2]);
            t_vec3 col1 = sq1->GetColor();
            t_vec3 col2 = sq2->GetColor();
            sq1->SetColor(col2);
            sq2->SetColor(col1);
            sq1->SetToDraw(true);
            sq2->SetToDraw(true);
            glutPostRedisplay();
            ProcessMatches();
        }
        else {
            printf("Not neighbors: (%d,%d) and (%d,%d)\n", row1, col1, row2, col2);
        }
        mSelectedIndex = -1;
    }
}

/* Function for static display callback
 * ARGS: None.
 * RETS: None.
 */
void Scene::StaticDisplay()
{
    if (mCurrentScene)
        mCurrentScene->Display();
}

/* Function for static keyboard callback
 * ARGS:
 *  (unsigned char) - key pressed
 *  (int) - mouse X
 *  (int) - mouse Y
 * RETS: None.
 */
void Scene::StaticKeyboard(unsigned char Key, int X, int Y)
{
    if (mCurrentScene)
        mCurrentScene->Keyboard(Key, X, Y);
}

/* Function for static timer callback
 * ARGS:
 *  (int) - timer value
 * RETS: None.
 */
void Scene::StaticTimer(int value)
{
    if (mCurrentScene)
        mCurrentScene->Timer(value);
}

/* Function for static reshape callback
 * ARGS:
 *  (int) - new width
 *  (int) - new height
 * RETS: None.
 */
void Scene::StaticReshape(int w, int h)
{
    if (mCurrentScene)
        mCurrentScene->Reshape(w, h);
}

/* Function for static mouse callback
 * ARGS:
 *  (int) - button
 *  (int) - state
 *  (int) - x
 *  (int) - y
 * RETS: None.
 */
void Scene::StaticMouse(int button, int state, int x, int y)
{
    if (mCurrentScene)
        mCurrentScene->Mouse(button, state, x, y);
}

/* Function for finding and marking matches (horizontal/vertical groups of >=3)
 * ARGS: None.
 * RETS:
 *  (bool) - true if any matches were found and marked
 */
bool Scene::FindAndMarkMatches() {
    bool anyMarked = false;
    bool marked[FSIZE][FSIZE] = { false };

    auto colorsEqual = [](const t_vec3& a, const t_vec3& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b;
        };

    for (int row = 0; row < FSIZE; ++row) {
        int len = 1;
        for (int col = 1; col <= FSIZE; ++col) {
            if (col < FSIZE) {
                Square* prev = static_cast<Square*>(mPrimitives[row * FSIZE + (col - 1)]);
                Square* cur = static_cast<Square*>(mPrimitives[row * FSIZE + col]);
                if (prev->IsToDraw() && cur->IsToDraw() && colorsEqual(prev->GetColor(), cur->GetColor())) {
                    ++len;
                }
                else {
                    if (len >= 3) {
                        printf("Deleted Group ");
                        for (int c = col - len; c < col; ++c) {
                            printf("X:%d,Y:%d ", c, row);
                        }
                        printf("\n");
                        for (int c = col - len; c < col; ++c) {
                            marked[row][c] = true;
                        }
                    }
                    len = 1;
                }
            }
            else {
                if (len >= 3) {
                    printf("Deleted Group ");
                    for (int c = col - len; c < col; ++c) {
                        printf("X:%d,Y:%d ", c, row);
                    }
                    printf("\n");
                    for (int c = col - len; c < col; ++c) {
                        marked[row][c] = true;
                    }
                }
            }
        }
    }

    for (int col = 0; col < FSIZE; ++col) {
        int len = 1;
        for (int row = 1; row <= FSIZE; ++row) {
            if (row < FSIZE) {
                Square* prev = static_cast<Square*>(mPrimitives[(row - 1) * FSIZE + col]);
                Square* cur = static_cast<Square*>(mPrimitives[row * FSIZE + col]);
                if (prev->IsToDraw() && cur->IsToDraw() && colorsEqual(prev->GetColor(), cur->GetColor())) {
                    ++len;
                }
                else {
                    if (len >= 3) {
                        printf("Deleted Group ");
                        for (int r = row - len; r < row; ++r) {
                            printf("X:%d,Y:%d ", col, r);
                        }
                        printf("\n");
                        for (int r = row - len; r < row; ++r) {
                            marked[r][col] = true;
                        }
                    }
                    len = 1;
                }
            }
            else {
                if (len >= 3) {
                    printf("Deleted Group ");
                    for (int r = row - len; r < row; ++r) {
                        printf("X:%d,Y:%d ", col, r);
                    }
                    printf("\n");
                    for (int r = row - len; r < row; ++r) {
                        marked[r][col] = true;
                    }
                }
            }
        }
    }

    for (int row = 0; row < FSIZE; ++row) {
        for (int col = 0; col < FSIZE; ++col) {
            if (marked[row][col]) {
                Square* sq = static_cast<Square*>(mPrimitives[row * FSIZE + col]);
                sq->SetToDraw(false);
                anyMarked = true;
                TryDropBonus(row, col, sq->GetColor());
            }
        }
    }

    if (anyMarked)
        glutPostRedisplay();
    return anyMarked;
}

/* Function for applying gravity and refilling empty cells with random colors
 * ARGS: None.
 * RETS: None.
 */
void Scene::ApplyGravityAndRefill() {
    for (int col = 0; col < FSIZE; ++col) {
        std::vector<t_vec3> visibleColors;
        for (int row = 0; row < FSIZE; ++row) {
            Square* sq = static_cast<Square*>(mPrimitives[row * FSIZE + col]);
            if (sq->IsToDraw()) {
                visibleColors.push_back(sq->GetColor());
            }
        }
        int newRow = 0;
        for (size_t i = 0; i < visibleColors.size(); ++i) {
            Square* sq = static_cast<Square*>(mPrimitives[newRow * FSIZE + col]);
            sq->SetColor(visibleColors[i]);
            sq->SetToDraw(true);
            ++newRow;
        }
        for (int row = newRow; row < FSIZE; ++row) {
            Square* sq = static_cast<Square*>(mPrimitives[row * FSIZE + col]);
            int randIdx = rand() % mColors.size();
            sq->SetColor(mColors[randIdx]);
            sq->SetToDraw(true);
        }
    }
    glutPostRedisplay();
}

/* Function for static timer callback for step-by-step match processing
 * ARGS:
 *  (int) - phase/depth parameter
 * RETS: None.
 */
void Scene::StepMatchTimerCallback(int depth) {
    if (mCurrentScene)
        mCurrentScene->StepMatchProcessing(depth);
}

/* Function for processing matches step by step (find, delete, gravity, bonuses)
 * ARGS:
 *  (int) - phase (0=find, 1=gravity, 2=bonuses)
 * RETS: None.
 */
void Scene::StepMatchProcessing(int phase) {
    if (phase == 0) {
        bool found = FindAndMarkMatches();
        if (!found) {
            mProcessingMatches = false;
            for (auto* p : mPrimitives) p->SetToDraw(true);
            glutPostRedisplay();
            return;
        }
        if (!mPendingBonuses.empty()) {
            glutTimerFunc(1000, StepMatchTimerCallback, 2);
        }
        else {
            glutTimerFunc(1000, StepMatchTimerCallback, 1);
        }
    }
    else if (phase == 1) {
        ApplyGravityAndRefill();
        glutTimerFunc(1000, StepMatchTimerCallback, 0);
    }
    else if (phase == 2) {
        ApplyPendingBonuses();
        glutPostRedisplay();
        glutTimerFunc(1000, StepMatchTimerCallback, 1);
    }
}

/* Function for starting match processing after a swap
 * ARGS: None.
 * RETS: None.
 */
void Scene::ProcessMatches() {
    if (mProcessingMatches) return;
    mProcessingMatches = true;
    StepMatchProcessing(0);
}

/* Function for attempting to drop a bonus from a deleted square
 * ARGS:
 *  (int) - row of deleted square
 *  (int) - column of deleted square
 *  (const t_vec3&) - original color of the deleted square
 * RETS: None.
 */
void Scene::TryDropBonus(int row, int col, const t_vec3& originalColor) {
    if (rand() % 15 != 0) return;

    BonusType type = (rand() % 2 == 0) ? BONUS_RECOLOR : BONUS_BOMB;

    std::vector<int> candidates;
    for (int dr = -3; dr <= 3; ++dr) {
        for (int dc = -3; dc <= 3; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr, nc = col + dc;
            if (nr >= 0 && nr < FSIZE && nc >= 0 && nc < FSIZE) {
                int idx = nr * FSIZE + nc;
                Square* sq = static_cast<Square*>(mPrimitives[idx]);
                if (sq->IsToDraw()) {
                    candidates.push_back(idx);
                }
            }
        }
    }
    if (candidates.empty()) return;

    int targetIdx = candidates[rand() % candidates.size()];
    int tgtRow = targetIdx / FSIZE, tgtCol = targetIdx % FSIZE;

    Bonus bonus;
    bonus.type = type;
    bonus.sourceIndex = row * FSIZE + col;
    bonus.targetIndex = targetIdx;
    bonus.originalColor = originalColor;

    if (type == BONUS_RECOLOR) {
        std::vector<int> extraCandidates;
        for (int dr = -3; dr <= 3; ++dr) {
            for (int dc = -3; dc <= 3; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nr = tgtRow + dr, nc = tgtCol + dc;
                if (nr >= 0 && nr < FSIZE && nc >= 0 && nc < FSIZE) {
                    int idx = nr * FSIZE + nc;
                    Square* sq = static_cast<Square*>(mPrimitives[idx]);
                    if (!sq->IsToDraw()) continue;
                    int r2 = idx / FSIZE, c2 = idx % FSIZE;
                    bool isNeighbor = (abs(r2 - tgtRow) + abs(c2 - tgtCol) == 1);
                    if (idx != targetIdx && !isNeighbor) {
                        extraCandidates.push_back(idx);
                    }
                }
            }
        }
        int count = std::min(2, (int)extraCandidates.size());
        std::random_shuffle(extraCandidates.begin(), extraCandidates.end());
        for (int i = 0; i < count; ++i) {
            bonus.bombIndices.push_back(extraCandidates[i]);
        }
    }
    else {
        std::vector<int> areaIndices;
        for (int dr = -3; dr <= 3; ++dr) {
            for (int dc = -3; dc <= 3; ++dc) {
                int nr = tgtRow + dr, nc = tgtCol + dc;
                if (nr >= 0 && nr < FSIZE && nc >= 0 && nc < FSIZE) {
                    int idx = nr * FSIZE + nc;
                    Square* sq = static_cast<Square*>(mPrimitives[idx]);
                    if (sq->IsToDraw()) {
                        areaIndices.push_back(idx);
                    }
                }
            }
        }
        if (areaIndices.empty()) return;
        std::random_shuffle(areaIndices.begin(), areaIndices.end());
        int bombCount = std::min(5, (int)areaIndices.size());
        for (int i = 0; i < bombCount; ++i) {
            bonus.bombIndices.push_back(areaIndices[i]);
        }
        bool hasTarget = false;
        for (int idx : bonus.bombIndices) if (idx == targetIdx) hasTarget = true;
        if (!hasTarget && bombCount > 0) {
            bonus.bombIndices[0] = targetIdx;
        }
    }

    mPendingBonuses.push_back(bonus);
}

/* Function for applying all pending bonuses (recolor or bomb)
 * ARGS: None.
 * RETS: None.
 */
void Scene::ApplyPendingBonuses() {
    for (const Bonus& bonus : mPendingBonuses) {
        int srcRow = bonus.sourceIndex / FSIZE;
        int srcCol = bonus.sourceIndex % FSIZE;
        int tgtRow = bonus.targetIndex / FSIZE;
        int tgtCol = bonus.targetIndex % FSIZE;
        const char* bonusName = (bonus.type == BONUS_BOMB) ? "BOMB" : "RECOLOR";
        printf("Bonus %s from X: %d, Y: %d to X: %d, Y: %d\n",
            bonusName, srcCol, srcRow, tgtCol, tgtRow);

        if (bonus.type == BONUS_RECOLOR) {
            std::vector<int> allIndices;
            allIndices.push_back(bonus.targetIndex);
            allIndices.insert(allIndices.end(), bonus.bombIndices.begin(), bonus.bombIndices.end());
            printf("  recolor to RGB(%d,%d,%d) for positions: ",
                bonus.originalColor.r, bonus.originalColor.g, bonus.originalColor.b);
            for (size_t i = 0; i < allIndices.size(); ++i) {
                int idx = allIndices[i];
                int row = idx / FSIZE, col = idx % FSIZE;
                printf("X:%d,Y:%d", col, row);
                if (i != allIndices.size() - 1) printf(", ");
            }
            printf("\n");

            Square* target = static_cast<Square*>(mPrimitives[bonus.targetIndex]);
            target->SetColor(bonus.originalColor);
            target->SetToDraw(true);
            for (int idx : bonus.bombIndices) {
                Square* sq = static_cast<Square*>(mPrimitives[idx]);
                sq->SetColor(bonus.originalColor);
                sq->SetToDraw(true);
            }
        }
        else if (bonus.type == BONUS_BOMB) {
            printf("  bomb destroys positions: ");
            for (size_t i = 0; i < bonus.bombIndices.size(); ++i) {
                int idx = bonus.bombIndices[i];
                int row = idx / FSIZE, col = idx % FSIZE;
                printf("X:%d,Y:%d", col, row);
                if (i != bonus.bombIndices.size() - 1) printf(", ");
            }
            printf("\n");
            for (int idx : bonus.bombIndices) {
                Square* sq = static_cast<Square*>(mPrimitives[idx]);
                sq->SetToDraw(false);
            }
        }
    }
    mPendingBonuses.clear();
}

